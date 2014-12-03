echo Test:fileDelete starts.
# create, open again, then close both
fileOpen /test 8
fileOpen /test 3
fileClose 1
fileClose 2
# open and delete
fileOpen /test 3
fileDelete 3
fileClose 3
# create and delete
fileOpen /test 8
fileDelete 4
fileClose 4

echo Test finished.
exit

