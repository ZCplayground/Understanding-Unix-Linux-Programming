Programming for device!

# Device is somewhat like file..

File has: data, attributes, filename in dir to be recognized. We can read data from file or write data into file. There is a similar pattern in device.

You speak to the microphone. Sound card converts your sound signal into data stream. Program in microphone can read data from the stream. When program write data into the data stream, sound card can do this procedure reversly and the sound is emitted from the speaker.

~~~
           your voice   +——————————+   
microphone --------->   |sound card| >>> datastream <----------  program
           write device +——————————+                read device

           
                                      +——————————+   
speaker ------------>  datastream <<< |sound card|  <----------  program
           read device                +——————————+  write device   

device: sound card
device likes file.
~~~

- Device has a filename.
- Device **support all system calls** that can be used in file.(open, read, close, stat, cat, mv, ln, ls ....)

## Terminal file test

Open a terminal and input:

~~~
$ tty
~~~

it may display:`/dev/pts/0`. It means that the device description file name corresponding to the terminal is `/dev/pts/0`.

Create a file 'test' input some data. Then open another terminal and input:

~~~
cp test /dev/pts/0
~~~

In this step you write some data into device description file`/dev/pts/0`. The corresponding terminal display these data in the screen.

# Attributes of device file

Device files are **links**, not containers. The **i-node** of the device file stores **a pointer to the kernel program.**

Use this command:

~~~
ls -li /dev/pts/0
~~~

It shows like:

~~~
3 crw--w---- 1 ruijie tty 136, 0  5月 20 20:10 /dev/pts/0
~~~

- i-node number is 3.
- File owner is ruijie, and group is tty.
- 136 indicates that the program for transmitting data at the terminal is a **subprogram** numbered 136 in the device-process table.
- 136 is Major device number, 0 is Secondary device number. 
- The Major device number decides the actual program (in the kernel) that process the device. Major device number tells location of code that reads data from device.
- Secondary device number is an argument which is conveyed to the subprogram.
- **File owners can read and write devices, whereas others can only write.**

## Device file and i-node

You can't tell device file from disk file according to filename in the directory. The difference in file types is reflected in i-node.

- Disk file's i-node contains a pointer to disk blocks.
- Device file's i-node contains a pointer to kernel subprogram list. 

We can examine how the system call `read` works. The kernel finds i-node of file descriptor, containing file's type. If it is a disk file, then the kernel reads data by accessing the block allocation table. If it is a device file, then the kernel calls file driver program to read data from device. All system calls(open, write, lseek, close, etc.) work in the similar pattern.

# The difference between them..

The system call `open` create a connection from file to device. The connection of disk file is different from the connection of device. Disk file need cache in kernel. Terminal doesn't need cache, in contrast, it needs to send data as fast as possible.

About connection's properties:

1. Which properties do connections have?
2. How to know the current property?
3. How to change the current property?

## Disk connection properties

fcntl - manipulate file descriptor

Use `man fcntl` to learn about how to manipulate the flags in file descriptor. For example the following code can turn off kernel buffering.

~~~
#include<fcntl.h>
int s;
s = fcntl(fd, F_GETFL);        // 1. get file status flags
s |= O_SYNC;                   // 2. set SYNC flag
result = fnctl(fd, F_SETFL, s) // 3. set flags
if(result == 1)
    perror("setting SYNC");
~~~

These three steps 

1. Read settings from the kernel to variables
2. Change these settings.
3. Return them to the kernel.

is the typical way to change connection properties in Unix/Linux.

## Terminal connection properties

Terminal is a kind of device.

The I/O of terminal is not simple as we think. The following program can show some infomation:

~~~
#include <stdio.h>

int main()
{
	int c, n = 0;
	while((c=getchar())!='Q')
		printf("char %3d is %c code %d\n", n++, c, c);
}

~~~

When you enter "hello" and its output is like:

~~~
hello
char   0 is h code 104
char   1 is e code 101
char   2 is l code 108
char   3 is l code 108
char   4 is o code 111
char   5 is 
 code 10
~~~

We can conclude that:

1. The input is buffered, not immediately sended to output.
2. When you enter ENTER, the program started to display these characters.
3. ENTER diplay ASCII code 10. But ENTER in the keyboard represent ASCII code 13.

So there must be a middle layer between file descriptors and process.

1. The process accepts data after user input an ENTER.
2. The process sees the  ENTER  as a ASCII code 10. (\n)

The set of kernel subprograms that handle the flow of data between processes and external devices is called a **device driver.**

Driver contains lots of device operating settings. The process can read, write, reset these setting flags through stty.

# stty command

You can use `stty` command to read or set terminal driver settings.
~~~
$ stty
$ stty -a
~~~

~~~
ruijie@ubuntu:~$ stty -a
speed 38400 baud; rows 31; columns 75; line = 0;
intr = ^C; quit = ^\; erase = ^?; kill = ^U; eof = ^D; eol = M-^?;
eol2 = M-^?; swtch = M-^?; start = ^Q; stop = ^S; susp = ^Z; rprnt = ^R;
werase = ^W; lnext = ^V; flush = ^O; min = 1; time = 0;
-parenb -parodd cs8 hupcl -cstopb cread -clocal -crtscts
-ignbrk brkint -ignpar -parmrk -inpck -istrip -inlcr -igncr icrnl ixon
-ixoff -iuclc ixany imaxbel iutf8
opost -olcuc -ocrnl onlcr -onocr -onlret -ofill -ofdel nl0 cr0 tab0 bs0 vt0
ff0
isig icanon iexten echo echoe echok -echonl -noflsh -xcase -tostop -echoprt
echoctl echoke
~~~

Some settings have int values(`rows`, `columns`, `line`. By the way, this attributes can be used for more functions in our own `more` command. It's insteresting, right?) and some other have bool value. For example `icrnl` means convert Carriage Return to NewLine. `-olcuc` means convert LowerCase to UpperCase. A minus sign before an attribute indicates that the attribute is turned off.


You can try these command:

~~~
$ stty erase X
$ stty  -echo
~~~

You can change the delete button to any desired button. You can make the input on the terminal not be echoed, which can protect it when you enter the password.

Now can we make our own version of `stty`?

1. Get these attributes and settings from driver.
2. Change it.
3. Send it back to driver.

~~~
$ man -k "terminal attributes"
~~~

See `echostate.c` and `setecho.c`

# Conclusion

The source or destination of any data is considered by Unix as a file. 

1. Device is somewhat like file. System calls are useful for both disk file and character device.
2. The difference between file and device. What makes them different is the connection. `<file | dir | char device>` In i-node mode.

Use `stty` to see or set attributes of device.

The fd(file descriptor) of disk file contains cache attributes. Terminal's fd contains many attrbutes definition code, such as echo, character conversion, etc.
