# test get and set APIs
fileOpen /test 8
# return fdId: 1
get 1
set 1 hello
get 1

fileDelete 1
fileClose 1

exit