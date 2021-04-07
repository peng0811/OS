#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

void fin(){
    int num;
    int *ans=0;
    printf("Input size: ");
    scanf("%d",&num);
    printf("ans: ");
    ans = malloc(sizeof(int)*num);
    ans[0]=0;
    ans[1]=1;
    printf("1 ");
    for(int i=0;i<num-1;i++){
        ans[i+2]=ans[i+1]+ans[i];
        printf("%d ",ans[i+2]);
    }
    printf("\n");
}


int main(){
    int status;
    pid_t PID=fork();

    switch (PID)
    {
    case -1:
        perror("fork()");
        exit(-1);
    case 0:
        printf("Child:child precess\n");
        fin();
        scanf("%d",&status);
        sleep(3);
        printf("Child process end\n");
        break;
    default:
        printf("Parent:parent process\n");
        wait(&status);
        printf("Parent process end\n");
        break;
    }
}
