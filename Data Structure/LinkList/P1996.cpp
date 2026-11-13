#include<bits/stdc++.h>
using namespace std;
typedef struct Node
{
    int num;
    struct Node *next;
}Node,*CLinkList;
CLinkList CL;
void InitCLinkList(CLinkList *CL)
{
    (*CL)=(CLinkList)malloc(sizeof(Node));
    (*CL)->next=*CL;
    (*CL)->num=0;
}
void CreateCLinkList(CLinkList CL,int n)
{
    Node *p,*s;
    p=CL;
    for (int i=1;i<=n;i++)
    {
	    s=(Node *)malloc(sizeof(Node));
        s->num=i;
        p->next=s;
        p=s;
    }
    p->next=CL->next;   
}
void Josephus(CLinkList CL,int n,int m)
{
    Node *p,*s;
    p=CL;
    while (n>1)
    {
        for(int i=1;i<m;i++) p=p->next;
        s=p->next;
        p->next=s->next;
        printf("%d ",s->num);
        n--;
    }
    printf("%d\n",p->num);
}
int main(){
    int n,m,x;
    cin>>n>>m;
    InitCLinkList(&CL);
    CreateCLinkList(CL,n);
    Josephus(CL,n,m);
    system("pause");
    return 0;
}