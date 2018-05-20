**ls -- list directory contents**

# Three questions when you build ls command:

1. How to read entry and show directory contents?
2. How to get file properties and how to show them?
3. Give you a name, how to tell whether it is a file or a directory? 

Hint: The **directory is a special kind of file**. It contains a lot of 
records, each with a uniform standard format. The content of 
each record represents a file or directory. The directory can 
be opened by a standard file function. The directory is not 
an unformatted text but contains a certain data structure.

# Learn how to program ls command:

1. man -k direct | grep read  # learn how to read dir file
2. read mannul of readdir and get other information.
3. learn about opendir/readdir/closedir/dirent.h and so on.
4. Write your own ls command!

~~~
    open directory
+-> read entry            - end of dir? -+
|_  display file info                    |
    close directory     <----------------+
~~~

see `ls1.c`

# ls -l: File size, file owner, etc.

Some other file attributes is not stored in dirent struct.

1. get file stat, see `filestat.c`
2. change the integer file mode into string like -rw-rw-r--. Use bit masking.
3. change user/group id into username and groupname.

You can do by these steps:
+--> man 3 stat
     man 3 getpwuid
     man -k uid | grep get
     man getgrgid

see `ls2.c` and use `ls2`

# Fix bug: ./ls2 /

ls2.c cannot use cannot use specified directory as argument. Try to fix this bug.

see `ls3.c` and `ls4.c`

# After finishing above, some features needed to completed as below:

1. sort: store all filename in an array and qsort it.
2. The standard ls is output in columns. The solution: read the file name into an array, and then calculate the column width and number of rows.
3. ls -a: Make ls output hidden files only when accepting option -a.
