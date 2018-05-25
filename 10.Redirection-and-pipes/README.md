# Standard I/O Stream

All Unix I/O redirection based on the priciple of standard data stream. There are 3 important file descriptors in Unix:

1. stdin  (fd = 0)
2. stdout (fd = 1)
3. stderr (fd = 2)

Unix assumes that file descriptor 0, 1, 2 have been opened, and use them to read, write, report errors.

The default connection of `stdin`,`stdout` and `stderr` is connected to **terminal**. So program will read data from keyboard and display output and error infomation to the screen.

Program is not aware of I/O direction. Program just read and write data from file descriptor. So if you change the fd, program will continue to send data while not realize that the source and the destination of the data has changed.

# Lowest - Available - fd 

The concept of fd is very simple: it is just a index of an array. Every process has a set of openned files. These files are saved in an array, and fd is the index.

**Lowest Available fd** is an important principle: When the file is opened, the **smallest available index** in the array is always arranged for this file as fd.

# How to redirect?

## 1. close then open

1. `close(0)`
2. `open(filename, O_RDONLY)`

After your `close(0)`, the connection to stdin is turned down. The current lowest available fd is 0, so the file you open will connect to stdin.

See `redirection1.c`


## open..close..dup..close

1. `yourfd = open(file)`
2. `close(0)`
3. `newfd = dup(fd)`
4. `close(yourfd)`

In the first step you open a file, but the fd is not 0. The next step is to `close(0)`. And then you duplicate `yourfd`. Because the current lowest available fd is 0, so the stdin is connect to disk file. Finally, `close(yourfd)`, only remain the `newfd` connection whose fd = 0.

# The same pattern in redirect output

After you `fork`, the child process prepare to `exec` run program. `exec` will replace the code and data but won't change the process' attributes and connections. So you can `close(1)` in child process. This will not affect parent process. Then child process should `creat("filename", 0644)`. The fd of `"filename"` is 1. The results of program will be written into `"filename"` while the program doesn't realize.
