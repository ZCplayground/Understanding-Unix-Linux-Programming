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
- Device support all system calls, commands related to file.(open, read, close, stat, cat, mv, ln, ls ....)

# Terminal file test

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
- 136 indicates that the program for transmitting data at the terminal is a subroutine numbered 136 in the device-process table.
- 136 is Major device number, 0 is Secondary device number.
- The Major device number decides the actual program that process the device. Secondary device number is an argument which is conveyed to the subroutine.
- **File owners can read and write devices, whereas others can only write.**


