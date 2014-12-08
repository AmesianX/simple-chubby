
SERVER_SRC := server/server.cc server/serverimpl.cc server/chubby_server.cc server/chubby_client.cc \
			  server/serverdb.cc \
			  server/serverdb_backstore.cc \
			  server/serverdb_paxos.cc \
			  server/sqlstmt.cc \
			  server/sqlite3.c


SERVER_OBJ := $(SERVER_SRC:.c=.o)
SERVER_OBJ := $(SERVER_OBJ:.cc=.o)
$(SERVER_OBJ): include/server.hh include/event.hh

server/%.o: server/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

server/%.o: server/%.cc
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $<

# Generate initial files to be edited
scaffold: include/server.x xdrpp
	$(XDRC) -serverhh -o server/serverimpl.hh $<
	$(XDRC) -servercc -o server/serverimpl.cc $<

server/server: $(SERVER_OBJ)
	$(LD) -o $@ $(LDFLAGS) $(SERVER_OBJ) $(LIBS) -Lpaxos -lpaxos

# test serverdb
server/serverdb: server/sqlite3.o server/sqlstmt.o server/serverdb.o
	$(LD) -o $@ $(LDFLAGS) $^ $(LIBS)
	rm -f test.db
	server/serverdb


server/serverimpl: server/sqlite3.o server/sqlstmt.o server/serverdb.o  server/serverimpl.o
	$(LD) -o $@ $(LDFLAGS) $^ $(LIBS)
	rm chubbystore.db
	server/serverimpl

