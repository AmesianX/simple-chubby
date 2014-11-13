// Scaffolding originally generated from include/server.x.
// Edit to add functionality.

#include "server/serverimpl.hh"

#include <iostream>
#include <set>
#include <vector>


// copied from include/client.h
enum ClientError {
    /*
     * Set, Get or Remove failed because they key was not found.
     */
    KEY_NOT_FOUND,
    /*
     * Create operation failed because the key has no parent.
     */
    NO_PARENT,
    /*
     * The key has children so it cannot be deleted.
     */
    HAS_CHILDREN,
    /*
     * The key path violates our formatting rules
     */
    MALFORMED_KEY,
};

std::string getParentKey(const std::string &key)
{
    // find the lenth of the parent of KEY
    int pos = key.length() - 1;
    while (pos > 0) { /* if pos==0, the parent is root, 
			 then break and return "/" */
	if (key[pos] == '/') break;
	--pos;
    }
    if (pos == 0)
	return "/";
    else 
	return std::string(key, 0, pos);  // create a substring of key
}


std::string getChildName(const std::string &parentKey, const std::string &childKey)
{
    int parLen = parentKey.length();
    int pos = parLen+1;
    while (pos < childKey.length()) {
	if (childKey[pos] == '/') break;
	pos++;
    }
    return std::string(childKey, parLen+1, pos - parLen); // create a substring of childKey
}

/* Returns true is C is any letter, numbers, underscore, or slash. */
bool checkChar(char c)
{
    std::string validChars = "_/0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    return validChars.find(c) != std::string::npos;
}

/* Paths must begin with a '/' and must contain only letters, numbers, 
   underscores, and single slashes to separate components. 
   Cannot be "/". Cannot end with '/'.*/
bool checkKey(const std::string &key)
{
    // key must begin with '/'
    if (key[0] != '/') return false;
    // path cannot end "/"
    if (key[key.length() - 1] == '/') return false;
    // no space
    for (int i = 0; i < key.length(); ++i)
    {
	// paths must contain only letters, numbers, underscores, and slashes to separate components
	if(!checkChar(key[i])) return false;
    }
    // path cannot contain "//"
    if(key.find("//") != std::string::npos) return false;
    return true;
}


std::unique_ptr<RPCBool>
api_v1_server::create(std::unique_ptr<kvpair> arg)
{
    std::unique_ptr<RPCBool> res(new RPCBool);
    std::string key = arg->key;
    std::string val = arg->val;

    // sanity checking (e.g. prevent malformed paths)
    if (!checkKey(key)) {
	// bad key
	res->discriminant(1);
	res->errCode() = MALFORMED_KEY;
	return res;
    }

    // check if the parent exists
    std::string parentKey = getParentKey(key);
	
    if(parentKey.compare("/") != 0   // parentKey is not root
       && !db.hasKey(parentKey)) {
	// no parent
	res->discriminant(1);
	res->errCode() = NO_PARENT;
    } else { 
	res->discriminant(0);
	if (db.hasKey(key)) {
	    res->val() = false;
	    std::cout << "Created " << key << " Failed" << std::endl;
	} else {
	    res->val() = true;
	    db.set(key, val);
	    std::cout << "Created " << key << " Succeded" << std::endl;
	}
    }
    return res;
}

std::unique_ptr<RPCBool>
api_v1_server::remove(std::unique_ptr<longstring> arg)
{
    std::unique_ptr<RPCBool> res(new RPCBool);
    std::string key = *arg;
  
    // sanity checking (e.g. prevent malformed paths)
    if (!checkKey(key)) {
	// bad key
	res->discriminant(1);
	res->errCode() = MALFORMED_KEY;
	return res;
    }
    
    if (db.hasKey(key)) {
	// check its sub-keys
	auto s = db.list(key);
	if(s.empty()) {
	    db.remove(key);
	    std::cout << "Removed " << key << " Succeded" << std::endl;
	    res->discriminant(0);
	    res->val() = true;
	} else {
	    // sub-key exists
	    res->discriminant(1);
	    res->errCode() = HAS_CHILDREN;
	}
    } else {
	// key not found
	res->discriminant(0);
	res->val() = false;
    }
    return res;
}

std::unique_ptr<RPCString>
api_v1_server::get(std::unique_ptr<longstring> arg)
{
    std::unique_ptr<RPCString> res(new RPCString);  
    std::string key = *arg;

    // sanity checking (e.g. prevent malformed paths)
    if (!checkKey(key)) {
	// bad key
	res->discriminant(1);
	res->errCode() = MALFORMED_KEY;
	return res;
    }
    // check key in db
    if (db.hasKey(key)) {
	res->discriminant(0);
	res->val() = db.get(key);
    } else {
	// key not found
	res->discriminant(1);
	res->errCode() = KEY_NOT_FOUND;
    }
    return res;
}

std::unique_ptr<RPCBool>
api_v1_server::set(std::unique_ptr<kvpair> arg)
{
    std::unique_ptr<RPCBool> res(new RPCBool);
    std::string key = arg->key;
    std::string val = arg->val;
  
    // sanity checking (e.g. prevent malformed paths)
    if (!checkKey(key)) {
	// bad key
	res->discriminant(1);
	res->errCode() = MALFORMED_KEY;
	return res;
    }
    if (db.hasKey(key)) {
	res->discriminant(0);
	db.set(key, val);
	res->val() = true;
	std::cout << "Modified " << key << " Succeded" << std::endl;
    } else {
	// key not found
	res->discriminant(1);
	res->errCode() = KEY_NOT_FOUND;
    }
    return res;
}

std::unique_ptr<RPCSet>
api_v1_server::list(std::unique_ptr<longstring> arg)
{
    std::unique_ptr<RPCSet> res(new RPCSet); 
    std::string key = *arg;

    if (key.compare("/") == 0) {
	// do 'list /' as a special case
	res->discriminant(0);
	SetString v;   // vector of strings
	for (auto childKey : db.list(""))
	    v.push_back(getChildName("", childKey));
	res->val() = v;
	return res;
    }
  
    // sanity checking (e.g. prevent malformed paths)
    if (!checkKey(key)) {
	// bad key
	res->discriminant(1);
	res->errCode() = MALFORMED_KEY;
	return res;
    }

    // check key in db
    if (db.hasKey(key)) {
	res->discriminant(0);
	SetString v;   // vector of strings
	for (auto childKey : db.list(key))
	    v.push_back(getChildName(key, childKey));
	res->val() = v;
    } else {
	// key not found
	res->discriminant(1);
	res->errCode() = KEY_NOT_FOUND;
    }
    return res;
}
