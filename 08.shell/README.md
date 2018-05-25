**A Program is a sequence of machine instructions stored in a file**. Running this program means loading the sequence of machine instructions into memory and letting the processor execute them one by one. An executable program is a sequence of machine instructions and its data.

**A Process is an instance of a running program**. It includes the memory space and settings when the program runs.

Program and data is stored in disk files. The program is running in the process.

Use `ps` to display the current process. It also has `-l`, `-a` options. 

`-f` means formatted output. User name replaces UID. Display complete command line in CMD.

~~~
ps -l
ps -fa
~~~

Option `-x` can list the kernal processes.

# Process management

We can see many attributes in process, such as UID, PID, PPID, priority, process state, tty, size of memory, etc. So we ask a question, where are these attributes? The situation is similar with file. The kernel manage files in disk. Is this similar to the management of the process?

file|process
---|---
data|executable code
file attributes|process attributes
The kernel create and delete files|Similarly
The kernel manage files in disk|The kernel manage processes in memory

We must mention some **memory management** knowledge here. To some extent it is similar to the disk sequence table in file management.

Now we only need to look at the memory as a large array without additional assumptions. **The entire system is divided into kernel space and user space.** Like blocks on disk, memory is also divided into small chunks, which are called **pages**. Like files that do not necessarily occupy continuous disk blocks, processes do not necessarily occupy continuous memory pages. Like file has a list of allocated disk blocks, process has a data structure of allocated memory pages. Creating a process is also a bit like creating a file. The kernal finds empty pages to store code and data, store allocated pages and attributes in specific data structure.

~~~
            kernel            Process B  
+------+    ↓----↓           ↙ ↓   ↓    ↘     
|memory|:   □□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□
+------+                 ↖ ↑    ↑    ↗ 
                         Process A
~~~

❤The operating system uses a file system to turn a cluster of disks into a tree directory structure. Its process system organizes silicon wafers into a process society. This is abstraction and is an important way to reduce complexity when constructing large projects.

# What is shell

Shell is a program that manage processes and run programs. Shell has 3 common functions:

1. run a program.(such as `ls`, `grep`)
2. manage input and output.(redirection)
3. programmable.(shell is a program language itself)

# How shell works

1. Get user input.
2. Shell create a new process to run this program.
3. Shell wait until the program finished.

If you want to write you own version of shell, you need to learn:

1. How to run a program.
2. How to establish a process.
3. How to wait.

# execvp, fork, wait

## How does a program run another program

The system call `exec` clears the current program's machine code and data, and the kernel load new program into the current process then try to run it.

It's a bit like changing your brains. Somebody may thinks, "I wish I could use Einstein's brain to figure out this problem, and then use my brain to do something else." Once your dream comes true, you will get Einstein's mind and talent, but your second wish is taken away along with your brain.

## How to create a new process

The system call `fork` can copy the process to a new one. The kernel do a list of things when `fork`:

1. allocate new pages and data structure in the kernal.
2. copy origin process into a new one.
3. add attributes.
4. **return control to two processes.**

Calling once, returning twice is a special feature of `fork` you never meet before. It's like when you press a button, there are two identical you in the world, but the first "you" and the second "you" will start a different life. 

Continue to use the above example as a analogy. As you can see, if the god gives Einstein's brain, it not only gives you Einstein's knowledge, but also clear all your thoughts in your mind.

The solution is copy yourself(no matter how you copy it). Once you successfully built your copy, you can put Einstein's brain into "its" head. You can still have your thoughts and plans.

Use `man fork` and you will learn more about the differences between parent/child process.

## How parent wait child process to finish

The parent call `wait` to wait its child process to finish. `wait` will **block** the program until the end of the child process, return the PID of the end process.

Use `man wait` to learn some options.

# Process programming

Think about how the entire system was built.

- call/return

A C program consists of many functions. A function can call another function, passing some parameter. The called funtion performs a specific operation returns a value. Each function has its local variables. They communicate through call/return system.(return values)

- exec/exit

A C program can `fork/exec` another program. THe called program performs a specific operation and return a value through `exit()` The parent process can use `wait()` to get the return value of `exit()`. The entire system is desiged to a set of small programs, instead of a big programs consists of a lot of functions.


# Write your shell

Use mannal to do it!

~~~
+---→ get command
|          |
|          ↓
|    +----fork----+
|    |            |
|    |            |
|  wait         execvp
|    ↓            ↓
+----+           exit
~~~
