**A semaphore is an integer whose value is never allowed to fall below zero.**  

Two  operations  can  be performed on semaphores: **increment** the
semaphore value by one (`sem_post(3)`); and **decrement** the semaphore value
by  one  (`sem_wait(3)`).  If the value of a semaphore is currently zero,
then a sem_wait(3) operation will block until the value becomes greater
than zero.

Named semaphores
    A named semaphore is identified by a name:
    form `/somename` a null-terminated string of up to NAME_MAX-4 (i.e.,
              251) characters consisting of an initial slash, followed by  one
              or  more  characters,  none of which are slashes.

Two processes can operate on the same named semaphore by passing the 
    same name to `sem_open(3)`.

The  `sem_open(3)` function creates a new named semaphore or opens
an existing named  semaphore.   
    
After  the  semaphore  has  been opened, it can be operated on using 
`sem_post(3)` and `sem_wait(3).`
When a process has finished using  the  semaphore,  it  can  use
`sem_close(3)`  to  close  the semaphore.  

When all processes have finished using the semaphore, it can be removed
from the  system using `sem_unlink(3).`


SEE ALSO
       sem_close(3),     sem_destroy(3),     sem_getvalue(3),     sem_init(3),
       sem_open(3), sem_post(3), sem_unlink(3), sem_wait(3), pthreads(7)

# open Named

sem_t *sem_open(const char *name, int oflag,
                       mode_t mode, unsigned int value);

name pattern: above

oflag:  If O_CREAT is specified in oflag, then the
       semaphore is created if it does not already exist. 

mode： The mode argument specifies the  permissions  to
       be  placed on the new semaphore, as for open(2).

       Both read and write permission should be granted to
       each class of user that will access the semaphore. 

vlaue: initial value for the new semaphore.

RETURN: On success, sem_open() returns the address of  the  new  semaphore;
       this  address  is  used  when calling other semaphore-related func‐
       tions.  On error, sem_open() returns SEM_FAILED, with errno set  to
       indicate the error.

# init Unnamed

 
int sem_init(sem_t *sem, int pshared, unsigned int value);


initializes the unnamed semaphore at the address pointed to by sem.  The value argument specifies the initial value for  the  semaphore. 

The  `pshared`  argument indicates whether this semaphore is to be shared between the threads of a process, or `between processes.`

If pshared has the value 0, then the semaphore is  shared  between  the threads  of  a  process,  and should be located at some address that isvisible to all threads 


If  pshared is `nonzero`, then the semaphore is shared between processes, and `should be located in a region of shared  memory`  (see  shm_open(3), mmap(2),  and  `shmget(2)`).   (Since a child created by fork(2) inherits its parent's memory mappings, it can also access the  semaphore.)   Any process  that  can  access  the shared memory region can **operate on the semaphore using** `sem_post(3)`, `sem_wait(3)`, etc.

RETURN VALUE
       sem_init() returns 0 on success; on error, -1 is returned,
