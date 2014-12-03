# try to create some files/dir
fileOpen /test/a 8
# returned FdId: 0
fileOpen /test 8
# returned FdId: 1
fileOpen /test/a 8
# returned FdId: 0
fileOpen /dir 4
# returned FdId: 2
fileOpen /dir/a 8
# returned FdId: 3
fileOpen /dir/b 8
# returned FdId: 4

# try to delete some files/dir
get 2
fileDelete 2
fileDelete 3
get 2
fileDelete 4
get 2
fileDelete 2

exit