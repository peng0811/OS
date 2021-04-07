#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_SEQUENCE 20

typedef struct{
    long fib_sequence[MAX_SEQUENCE];
    int sequence_size;
}shared_data;

void fin(shared_data *op){
    int num;
    int *ans=malloc(sizeof(int)*op->sequence_size);
    ans[0]=0;
    ans[1]=1;
    op->fib_sequence[0]=1;
    for(int i=0;i<op->sequence_size;i++){
        ans[i+2]=ans[i+1]+ans[i];
        op->fib_sequence[i+1] = ans[i+2];
    }
}
int main(void)
{   
    int status;
    int shmid;
    pid_t pid;
    int *shm;
    shared_data *op; 
   
    
    shmid = shmget((key_t)1234,  sizeof(shared_data), IPC_CREAT | 0666);
    if (shmid < 0) {
        perror("shmget error");
        exit(-1);
    }
   
    
    shm = shmat(shmid, NULL, 0);
    if (shm == (int *)-1) {
        perror("shmat error");
        exit(-1);
    }

    op = (shared_data *)shm;
    printf("size:");
    scanf("%d",&op->sequence_size);
    if(op->sequence_size > MAX_SEQUENCE){
        printf("too larger");
        exit(-1);
    }

    pid = fork();

    switch(pid){
        case 0:printf("Child start\n");
                fin(op);
                scanf("%d",&status);
                sleep(3);
                printf("Child end\n");
                shmdt(shm);
                break;
        case -1:
                perror("fork()");
                exit(-1);
        default:printf("Parent start\n");
                wait(&status);
                printf("ans: ");
                for(int i=0;i<op->sequence_size;i++){
                    printf("%ld ",op->fib_sequence[i]);
                }
                printf("\n");
                printf("end\n");
                shmdt(shm);
                shmctl(shmid,IPC_RMID,NULL);
                break;

    }


    return 0;
}