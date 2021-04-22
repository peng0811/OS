#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>


#define INTERVAL 10000
int circle_points=0,square_points=0;

void *PI(void *param){
    printf("Start to count the number of points\n");
    double rand_x,rand_y,origin_dist;
    for(int i=0;i<INTERVAL*INTERVAL;i++){
        rand_x=(double)(rand()%(INTERVAL+1))/INTERVAL;
        rand_y=(double)(rand()%(INTERVAL+1))/INTERVAL;
        origin_dist = rand_x * rand_x + rand_y * rand_y;
        if(origin_dist <= 1)circle_points++;
        square_points++;
    }
    pthread_exit(0);
}

int main(int argc,char *argv[]){
    srand(time(NULL));
    double pi;
    pthread_t tid;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_create(&tid,&attr,PI,argv[1]);
    pthread_join(tid,NULL); 
    
    pi =((double)(4*circle_points)/square_points);
    printf("ans=%f\n",pi);
}