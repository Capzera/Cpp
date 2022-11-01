#include<bits/stdc++.h>
#define OK 1
#define ERROR 0
using namespace std;
typedef struct Node
{
    int data;
    Node *next;
}Node,*Linklist;
Linklist LL;
void InitList(Linklist *LL)
{
    (*LL)=(Linklist)malloc(sizeof(Node));
    (*LL)->next=NULL;
}
void CreateList(Linklist LL,int n)
{
    Node *p,*s;
    int i,x;
    p=LL;
    for(i=1;i<=n;i++)
    {
        s=(Node *)malloc(sizeof(Node));
        scanf("%d",&x);
        s->data=x;
        p->next=s;
        s->next=NULL;
        p=s;
    }
}
int InsList(Linklist LL,int i,int x)
{
    Node *p,*s;
    int k=0;
    p=LL;
    while(p!=NULL&&k<i-1){
        p=p->next;
        k++;
    }
    if(k!=i-1) return ERROR;
    s=(Node *)malloc(sizeof(Node));
    s->data=x;
    s->next=p->next;
    p->next=s;
    return OK;
}
int DelList(Linklist LL,int i,int *e)
{
    Node *p,*r;
    int k=0;
    p=LL;
    while (p->next!=NULL&&k<i-1)
    {
        p=p->next;
        k++;
    }
    if(k!=i-1) return ERROR;
    r=p->next;
    p->next=r->next;
    free(r);
    return OK;
}
void PrintList(Linklist LL,int n)
{
    int i;
    Node *p;
    p=LL;
    for(i=1;i<=n;i++){
        p=p->next;
        printf("%d ",p->data);
    }
}
int main(){
    InitList(&LL);
    int n;
    scanf("%d",&n);
    CreateList(LL,n);
    PrintList(LL,n);
    system("pause");
    return 0;
}