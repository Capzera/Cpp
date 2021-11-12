#include<stdio.h>
#include<malloc.h>
#include<process.h>
#define OK 1
#define ERROR 0
#define OVERFLOW -2
#define MAXQSIZE 100
typedef int Status;
typedef int QElemType;
typedef struct{
	QElemType *base;
	int front;
	int rear;
}SqQueue;
Status InitQueue(SqQueue &Q){
	Q.base=(QElemType *)malloc(MAXQSIZE *sizeof(QElemType));
	if(!Q.base) exit(OVERFLOW);
	Q.front=Q.rear=0;
	return OK;
}
Status EnQueue(SqQueue &Q,QElemType e){
	if((Q.rear+1)%MAXQSIZE==Q.front) return ERROR;
	Q.base[Q.rear]=e;
	Q.rear=(Q.rear+1)%MAXQSIZE;
	return OK;
}
Status DeQueue(SqQueue &Q,QElemType &e){
	if(Q.front==Q.rear) return ERROR;
	e=Q.base[Q.front];
	Q.front=(Q.front+1)%MAXQSIZE;
	return OK;
}
