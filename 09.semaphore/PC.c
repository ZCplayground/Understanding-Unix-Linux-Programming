#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <semaphore.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define EMPTY 0    // 0 means empty,
#define PRODUCT 1  // 1 means there is a product

sem_t *sem_buf; // shared memory of semaphores
int *warehouse;
int k; // size of warehouse

// shared memory ID
int shmID1; // share semaphores between processes
int shmID2; // share warehouse between processes

//semaphores
sem_t * mutexP;           // P lock, initialize: 1
sem_t * mutexC;           // C lock, initialize: 1
sem_t * empty;            // P need empty to create product, initialize: 5
sem_t * product;          // C need product to take away, initialize: 0

void init();
void Producer();
void Consumer();

int main()
{
    printf("Input the warehouse buf size: ");
    scanf("%d", &k);
    init();

    while(1){
        srand((unsigned)time(NULL));
        int pid = fork();
        if(pid == -1){
            perror("fork");
            return -1;
        }
        else if(pid == 0){ // child
            int choice;
            choice = rand()%2;
            if(choice == 0)
                Producer();
            else if(choice == 1)
                Consumer();
            exit(0);
        }
        else{ // parent
            //int sleeptime = rand() % 3 + 1;  // randomly sleep 1~3 second(s)
            //sleep(sleeptime);
            sleep(1);
        }
    }
}

void init()
{
    shmID1 = shmget(IPC_PRIVATE, sizeof(sem_t)*4,  IPC_CREAT | 0666); // get a shared memory ID, 0666 means can read and write.
    sem_buf = (sem_t *)shmat(shmID1, NULL, 0); // attach shared memory identified by ID to address pf calling process
    shmID2 = shmget(IPC_PRIVATE, sizeof(int)*k, IPC_CREAT | 0666);
    warehouse = (int *)shmat(shmID2, NULL, 0); // NULL: automaticlly allocated. 0: read and write.

    int i;
    for(i = 0; i < k; i++)
        warehouse[i] = EMPTY;

    mutexP = sem_buf;
    mutexC = sem_buf+1;
    empty = sem_buf+2;
    product = sem_buf+3;

    sem_init(mutexP, 1, 1);
    sem_init(mutexC, 1, 1);
    sem_init(empty, 1, 5);
    sem_init(product, 1, 0);
}

void Producer()
{
    printf("ProducerID: %d starts.\n", getpid());

    sem_wait(empty);   // P needs a empty position to create product
    sem_wait(mutexP);  // avoid two P put into the same position simultaneously

    int i;
    for(i=0;i<k;i++) if(warehouse[i] == EMPTY) break;
    warehouse[i] = PRODUCT;

    sem_post(mutexP);  // avoid two P put into the same position simultaneously
    sem_post(product); // P puts a product for C to take away

    int posi = i + 1;
    printf("ProducerID: %d puts an product to %d\n", getpid(), posi);

    return;
}

void Consumer()
{
    printf("ConsumerID: %d starts.\n", getpid());

    sem_wait(product);  // C needs a product to take away
    sem_wait(mutexC);   // avoid two C take away the same product simultaneously

    int i;
    for(i=0;i<k;i++) if(warehouse[i] == PRODUCT) break;
    warehouse[i] = EMPTY;

    sem_post(mutexC);   // avoid two C take away the same product simultaneously
    sem_post(empty);    // C takes away product, that position become empty.

    int posi = i + 1;
    printf("ConsumerID: %d takes away the product from position %d\n", getpid(), posi);

    return;
}

// notes:

// 1. Link with -pthread: gcc -pthread PC.c -o PC
// in functions Producer(), Consumer(): The calling sequence of two sem_wait()/sem_post() functions must strictly obey the order in the code, otherwise it may deathlock.


