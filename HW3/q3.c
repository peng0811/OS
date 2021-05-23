#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#define INTERVAL 10000
int circle_points=0,square_points=0;
double buffer[5000][2];

typedef struct gen_point{
    int index;
    pthread_mutex_t lock;
}gen_point;

void *PI(void *P){
    pthread_mutex_lock(&(((gen_point*)P)->lock));
    printf("Start to generate point in %d pthread\n",((gen_point*)P)->index);
    double rand_x,rand_y;
    P = (gen_point*)P;
    for(int i=0;i<1000;i++){
        rand_x=(double)(rand()%(INTERVAL+1))/INTERVAL;
        rand_y=(double)(rand()%(INTERVAL+1))/INTERVAL;
        buffer[((gen_point*)P)->index*1000+i][0] = rand_x;
        buffer[((gen_point*)P)->index*1000+i][1] = rand_y;
    }
    ((gen_point*)P)->index++;
    pthread_mutex_unlock(&(((gen_point*)P)->lock)); 
    pthread_exit(0);
}

int main(int argc,char *argv[]){
    srand(time(NULL));
    double pi;
    pthread_t tid[5];
    pthread_attr_t attr;
    gen_point g;
    pthread_mutex_init(&(g.lock),NULL);
    g.index = 0;
    pthread_attr_init(&attr);
    pthread_create(&tid[0],&attr,PI,&g);
    pthread_create(&tid[1],&attr,PI,&g);
    pthread_create(&tid[2],&attr,PI,&g);
    pthread_create(&tid[3],&attr,PI,&g);
    pthread_create(&tid[4],&attr,PI,&g);
    pthread_join(tid[0],NULL);
    pthread_join(tid[1],NULL);
    pthread_join(tid[2],NULL);
    pthread_join(tid[3],NULL);
    pthread_join(tid[4],NULL);

    for(int i =0;i < 5000;i++){
        double x = buffer[i][0];
        double y = buffer[i][1];
        double c = x * x + y * y;
        if(c <= 1)circle_points++;
        square_points++;
    }
    pi =((double)(4*circle_points)/square_points);
    printf("%d c_point\n",circle_points);
    printf("ans=%f\n",pi);
    pthread_mutex_destroy(&g.lock);
}