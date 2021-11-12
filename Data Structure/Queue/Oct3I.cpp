#include<stdio.h>
#include<malloc.h>
#include<process.h>
#define OK 1
#define ERROR 0
#define OVERFLOW -2
typedef int Status;
typedef int QElemType;

typedef struct QNode{
	QElemType data;
	struct QNode *next;
}QNode,*QueuePtr;
typedef struct LinkQueue{
	QueuePtr front;
	QueuePtr rear;
}LinkQueue;

Status InitQueue(LinkQueue &Q){//构造空队列 
	Q.front=(QueuePtr)malloc(sizeof(QNode));
	if(!Q.front)exit(OVERFLOW);
	Q.front->next=NULL;
	Q.rear=Q.front;
	return OK;
}

Status EnQueue(LinkQueue &Q,QElemType e){//插入元素 
	Q.rear->data=e; 
    Q.rear->next=(QueuePtr)malloc(sizeof(QElemType));
    Q.rear=Q.rear->next; 
    Q.rear->next=NULL; 
    return OK;
}

Status DeQueue(LinkQueue &Q,QElemType &e){//删除队头并返回其值 
	if(Q.front==Q.rear) return ERROR;
	e=Q.front->data;
	Q.front=Q.front->next;
	return OK; 
}

Status DestroyQueue (LinkQueue &Q) { 
    if (!Q.front) 
        return ERROR; 
    Q.rear == NULL; 
    free (Q.front);    
    return OK;
}

int main(){
	LinkQueue que;
	int e,a,b,i=0,j;
	int m[50];
	InitQueue(que);
	printf("输入一个十进制数："); 
	scanf("%d",&a);
	printf("转换成几进制？（输入数字）");
	scanf("%d",&b);
	while (a!=0)
	{
		e=a%b;
		a/=b;
		EnQueue(que,e);
		i++;
	}
	j=i;
	while(i>0)
	{
		DeQueue(que,e);
		m[i]=e;
		i--;
	}
	if (b==16) printf("0x");
	if (b==8) printf("0");
	for(i=1;i<=j;i++)
	{
		if(m[i]>=10) printf("%c",m[i]+55);
		else printf("%d",m[i]);
	}
	system("pause");
	return 0;
}