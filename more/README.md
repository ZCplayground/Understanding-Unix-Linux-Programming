**more -- paging through text one screenful at a time.**

# How to Study Unix/Linux Programming?

The author proposes a very good method, is to **inspect those commands** that are commonly used in Unix/Linux OS **and see if we can implement one by ourselves.**

1. What can this command do?
2. How does it work and how is it achieved?
3. Can I write my own?

# What can this command do?

Run this command `more` in your terminal. Use `man more` to learn more details.

More will show the content of the first screen. At the bottom of the screen, the percentage of the file is displayed in white color text box. If you press SPACE, the content of the next screen is displayed. If you press ENTER, the next line is displayed. If you press q then exit.

# How does it work and how is it achieved?

Observe carefully, it is not difficult to get these procedure:

~~~
+----> show <24> lines from input
| +--> print [more?] message
| |    input Enter, SPACE, or q
| +--> if Enter, advance next page
+ ---- if Space, advance one line
       if q --> exit
~~~

# Can I write my own?

In the first try, we can make it easy to set a fixed number of output lines. 

See `more01.c`

~~~
gcc more01.c -o more01
~~~

**fix bug: How to get keyboard input?**

When you use `ls /bin more01`, it meets a bug.

The previous command redirects standard input of `more` to `ls`'s standard output. However, the code uses `getchar()` which is inappropriate and should read the keyboard input.

The file `/dev/tty` is a device description file for keyboards and displays. Reading this file is equivalent to getting user input from the keyboard.

See `more2.c`

# After finishing intial version

In fact, system programming is not so difficult, right? For realizing very basic functions, yes.

But if you want to achieve perfect functions, you have to work hard.

The list below you can consider adding to future version:

1. How to remove 'more?' in the bottom of each output?
2. How to get next output without enter after input q SPACE or ENTER?
3. How to calculate the percentage?
