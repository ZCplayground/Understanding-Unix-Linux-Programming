**cp: copy file**

# A good practice to learn C file functions.

~~~
open sourcefile for reading
open copyfile for writing
+-> read from source -- eof?  --+
|_  write from buffer to copy   |
                                |
    close two files <-----------+
~~~
