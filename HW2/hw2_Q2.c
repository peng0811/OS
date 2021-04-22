#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
typedef struct{
    int left;
    int right;
}parameters;
int num=0;
int *arr=0;

void merge(int l, int m, int r)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;

    int L[n1], R[n2];
 
    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];
 
    i = 0; 
    j = 0; 
    k = l; 
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        }
        else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}
void mergeSort(int l, int r)
{
    if (l < r) {
        int m = l + (r - l) / 2;
        mergeSort(l, m);
        mergeSort(m + 1, r);
        merge(l, m, r);
    }
}

void *sub(void *para){
    parameters *P=(parameters *)para;
    mergeSort(P->left,P->right);
    pthread_exit(0);
}

void *mergesub(void *param)
{
    parameters *P=(parameters *)param;
    int l=P->left;
    int r=P->right;
    int m=P->right/2;
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;

    int L[n1], R[n2];
 
    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];
 
    i = 0; 
    j = 0; 
    k = l; 
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        }
        else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }
 
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }
 
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

int main(int argc,char *argv[]){
    srand(time(NULL));
    pthread_t tid[3];
    pthread_attr_t attr;
    num = rand()%(20)+1;
    arr=malloc(sizeof(int)*(num));
    printf("original : ");
    for(int i=0;i<num;i++){
        arr[i] = rand()%(200)-100;
        printf("%d ",arr[i]);
    }
    printf("\n");
    parameters **P=malloc(sizeof(parameters *)*3);
    for(int i=0;i<3;i++)P[i]=malloc(sizeof(parameters));
    P[0]->left=0;
    P[0]->right=(num-1)/2;
    P[1]->left=(num-1)/2+1;
    P[1]->right=num-1;
    P[2]->left=0;
    P[2]->right=num-1;

    pthread_attr_init(&attr);
    pthread_create(&tid[0],&attr,sub,(void *)P[0]);
    pthread_create(&tid[1],&attr,sub,(void *)P[1]);
    pthread_join(tid[0],NULL);
    pthread_join(tid[1],NULL);

    pthread_create(&tid[2],&attr,mergesub,(void *)P[2]);
    pthread_join(tid[2],NULL);

    printf("after merge : ");
    for(int i=0;i<num;i++)printf("%d ",arr[i]);
    printf("\n");
}