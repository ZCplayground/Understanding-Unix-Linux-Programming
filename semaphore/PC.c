#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <semaphore.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#define EMPTY 0
#define PRODUCT 1

sem_t *sem_buf; // shared memory of semaphores
int *warehouse;

int k; // size of warehouse

int shmID1;
int shmID2;

void Producer();
void Consumer();

//semaphores
sem_t * mutexP;           // P lock, initialize: 1
sem_t * mutexC;           // C lock, initialize: 1
sem_t * empty;            // P need empty to create product, initialize: 5
sem_t * product;          // C need product to take away, initialize: 0


int main()
{
    shmID1 = shmget(IPC_PRIVATE, sizeof(sem_t)*4,  IPC_CREAT | 0666); // get a shared memory ID, 0666 means can read and write.
    sem_buf = (sem_t *)shmat(shmID1, NULL, 0); // attach shared memory identified by ID to address pf calling process
    mutexP = sem_buf;
    mutexC = sem_buf+1;
    empty = sem_buf+2;
    product = sem_buf+3;

    sem_init(mutexP, 1, 1);
    sem_init(mutexC, 1, 1);
    sem_init(empty, 1, 5);
    sem_init(product, 1, 0);

    printf("input the warehouse buf size: ");
    scanf("%d", &k);

    shmID2 = shmget(IPC_PRIVATE, sizeof(int)*k, IPC_CREAT | 0666);
    warehouse = (int *)shmat(shmID2, NULL, 0); // NULL: automaticlly allocated. 0: read and write.
    int i;
    for(i=0;i<k;i++) warehouse[i] = EMPTY; // 0 means empty, 1 means there is a product

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
        else // parent
            sleep(2);
    }
}

void Producer()
{
    printf("ProducerID: %d start.\n", getpid());

    sem_wait(mutexP);  // avoid two P put into the same position simultaneously
    sem_wait(empty);   // P needs a empty position to create product

    int i;
    for(i=0;i<k;i++) if(warehouse[i] == EMPTY) break;
    warehouse[i] = PRODUCT;

    sem_post(product); // P puts a product for C to take away
    sem_post(mutexP);  // avoid two P put into the same position simultaneously

    int posi = i + 1;
    printf("P put an product to %d\n", posi);

    return;
}

void Consumer()
{
    printf("ConsumerID: %d start.\n", getpid());

    sem_wait(mutexC);   // avoid two C take away the same product simultaneously
    sem_wait(product);  // C needs a product to take away

    int i;
    for(i=0;i<k;i++) if(warehouse[i] == PRODUCT) break;
    warehouse[i] = EMPTY;

    sem_post(empty);    // C takes away product, that position become empty.
    sem_post(mutexC);   // avoid two C take away the same product simultaneously

    int posi = i + 1;
    printf("C take away the product from position %d\n", posi);

    return;
}
