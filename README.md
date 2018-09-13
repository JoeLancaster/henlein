# henlein
Henlein (Named after Peter Henlein) is a small tool that watches for file system events and runs arbitrary commands based on said events. Written in C with the Linux `inotify` library.

# usage
`./hen file1 file2... command trigger`

# example
`./hen main.c make IN_CLOSE_WRITE`

`./hen *.c make IN_CLOSE_WRITE`
