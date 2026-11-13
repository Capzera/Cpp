/* 机长大大 Capzera*/
#include<bits/stdc++.h>
#include<conio.h>
#define TRUE 1
#define FALSE 0
#define MAXSIZE 100000
using namespace std;
typedef struct 
{
    int elem[MAXSIZE+1];
    int front;
    int rear;
}CircleQueue;
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
void CreateStack(CircleQueue *Q)
{
    int i,n,x;
    printf("请输入你想要在队列中预置几个元素:");
    scanf("%d",&n);
    system("cls");
    printf("请用空格隔开输出这%d个数:\n",n);
    for(i=1;i<=n;i++)
    {
        scanf("%d",&x);
        EnterQueue(Q,x);
    }
    system("cls");
    printf("循环队列创建成功！\n\n\n\n\n");
    system("pause");
}
void PrintQueue(CircleQueue Q)
{
    int i=Q.front;
    printf("对头：");
    while(i!=Q.rear){
        printf("%d ",Q.elem[i]);
        i=(i+1)%MAXSIZE;
    }
    printf("：队尾\n\n\n\n\n");
}
int main(){
    char ch1;
    int z=0,x;
    ch1='1';
    while(ch1!='5')
    {
        system("cls");
        printf("-----By:机长大大-----\n");
        printf("请选择你要使用的功能:\n");
        printf("1.初始化循环队列\n");
        printf("2.加入元素\n");
        printf("3.弹出元素\n");
        printf("4.打印队列\n");
        printf("5.退出\n");
        ch1=getch();
        switch(ch1)
        {
            case '1':
                system("cls");
                InitQueue(&Q);
                CreateStack(&Q);
                z=1;
            break;
            case '2':
                system("cls");
                if(!z){
                    printf("你还未创建队列!\n\n\n\n\n");
                    system("pause");
                    break;
                }
                printf("请输入你要加入队列的数值：\n");
                scanf("%d",&x);
                EnterQueue(&Q,x);
            break;
            case '3':
                system("cls");
                if(!z){
                    printf("你还未创建队列!\n\n\n\n\n");
                    system("pause");
                    break;
                }
                DeleteQueue(&Q);
                printf("弹出成功！\n\n\n\n\n");
                system("pause");
            break;
            case '4':
                system("cls");
                if(!z){
                    printf("你还未创建队列!\n\n\n\n\n");
                    system("pause");
                    break;
                }
                PrintQueue(Q);
                system("pause");
            break;
            case '5':
            break;
        }
    }
    system("cls");
    printf("欢迎下次使用.\n\n\n\n");
    system("pause");
    return 0;
}