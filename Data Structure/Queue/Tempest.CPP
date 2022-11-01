/* 机长大大 Capzera 循环队列*/
#include<bits/stdc++.h>
#include<conio.h>
#define TRUE 1
#define FALSE 0
#define MAXSIZE 100000
using namespace std;
struct CircleQueue
{
    int elem[MAXSIZE+1];
    int front;
    int rear;
};
CircleQueue Q;
void InitQueue(CircleQueue *Q)
{
    Q->front=Q->rear=1;
}
int EnterQueue(CircleQueue *Q,int x)
{
    if ((Q->rear+1)%MAXSIZE==Q->front) return FALSE;
    Q->elem[Q->rear]=x;
    Q->rear=(Q->rear+1)%MAXSIZE;
    return TRUE;
}
int DeleteQueue(CircleQueue *Q)
{
    if (Q->front==Q->rear) return FALSE;
    Q->front=(Q->front+1)%MAXSIZE;
    return TRUE;
}
void CreateQueue(CircleQueue *Q)
{
    int i,n,x;
    scanf("%d",&n);
    for(i=1;i<=n;i++)
    {
        scanf("%d",&x);
        EnterQueue(Q,x);
    }
}
void PrintQueue(CircleQueue Q)
{
    int i=Q.front;
    while(i!=Q.rear){
        printf("%d ",Q.elem[i]);
        i=(i+1)%MAXSIZE;

 .
 3   }
    printf("\n");
}
int main()
{
    InitQueue(&Q);
    CreateQueue(&Q);
    PrintQueue(Q);
    system("pause");
    return 0;
}