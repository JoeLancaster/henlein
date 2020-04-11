# henlein
Henlein is a small tool that watches for file system events and runs arbitrary commands based on said events. Written in C with the Linux `inotify` library.

# usage
`./hen file1 file2... command trigger`

# example
Run `make` when main.c is saved:

`./hen main.c make IN_CLOSE_WRITE`



Run `make` command when any .c file is saved:

`./hen *.c make IN_CLOSE_WRITE`




Run a renaming script when a file is added to `~/Downloads`:

`./hen ~/Downloads /usr/bin/renamestuff IN_CREATE`
