**pwd -- display the name of current/working directory**

# File System

~~~
             +---- file content
             |
file system -+---- file attributes: stored in i-node. 
             |                      Every file has a i-node struct,
             |                      store file attributes, blocks the file uses, etc.
             |
             +---- directory: dir is a kind of file(yes it has a i-node). 
                              Its content is a list of (i-node, filename) entries.

              i-node table: The list of all i-node. 
+----------+     ↓----↓      CONTENT: real "files"
|disk block|:   □□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□
+----------+    ↑
           super block: store info of file system.
~~~

# How to create a file?

We create a file named "list". Its content needs 3 disk blocks to be stored.

1. Kernel finds a empty i-node(for example the i-node number is 47) and store
   file attributes in it.
2. Allocate the disk block that stores the contents of the file. Assume that 
   the system allocates three blocks numbered 627, 200, 992. Kernel stores file 
   content in these blocks.
3. The kernel records the above block sequence(627,200,992) on the i-node.
4. Add the file name to the directory. As mentioned earlier, the directory is 
   also a file. Add the (47, list) record to the contents of the directory. 
   The correspondence between the file name and the i-node associates 
   the file name with the file content and attributes.

# What happen when we read from a file?

What happen when we input this command?

~~~
$ cat list
~~~

1. The kernel looks for entry containing the filename "list" in the directory 
   file, and get file's i-node number is 47.
2. Locate i node and read its contents. The kernel knows which disk blocks the 
   file contents are stored on.(627,200,992)
3. Read the corresponding data blocks.

# The true meaning of "file is in the directory" --> Link.

- From user's view, file "list" is in the directory "dir".
- From system's view, "list" has a i-node number 491. And the directory file has an entry (491, list).
- In other words: In the directory "dir" there is a link to the i-node 491, the attached name is "list".
 
1. The essence of the file is the file attributes in the i-node and the file content in a series of blocks.
2. The directory contains references to files, and each reference is called a link.
3. A link is a reference to an i-node and is bound to a name, that is "filename".
4. File only have i-node. File does not have a "name" but Link does. Kernel records the numbers of a file's links.

# The true meaning of "The directory is in the parent directory", "Directory contains subdirectories".

- From user's view, "a" is a subdirectory of "list" .
- From system's view, "list" contains a entry of (a's i-node, a).

The kernel sets an i-node in each directory pointing to itself, called "."
The kernel sets an i-node in each directory pointing to its parent dir, called ".."


# Related System Call

mkdir, rmdir, rm, ln, mv, cd
