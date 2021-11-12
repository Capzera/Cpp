#include<bits/stdc++.h>
using namespace std;
typedef struct Node
{
    int num;
    int pwd;
    struct Node *next;
}Node,* CLinkList;
CLinkList CL;
void InitCLinkList(CLinkList *CL)
{
    (*CL)=(CLinkList)malloc(sizeof(Node));
    (*CL)->next=*CL;
    (*CL)->num=0;
}
void CreateCLinkList(CLinkList CL,int i,int x,int n)
{
    Node *p,*s;
    p=CL;
    while(1==1){
        if (p->num==i-1) break;
        p=p->next;
    }
    s=(Node *)malloc(sizeof(Node));
    s->pwd=x;
    s->num=i;
    p->next=s;
    if (i==n) s->next=CL->next;

}
void Josephus(CLinkList CL,int m,int n)
{
    Node *p,*s;
    p=CL;
    while (n>1)
    {
        for(int i=1;i<m%n;i++) p=p->next;
        s=p->next;
        p->next=s->next;
        m=s->pwd;
        printf("%d ",s->num);
        n--;
    }
    printf("%d",p->num);
}
int main(){
    int i,n,m,x;
    cin>>n>>m;
    InitCLinkList(&CL);
    for(int i=1;i<=n;i++){
        cin>>x;
        CreateCLinkList(CL,i,x,n);
    }
    Josephus(CL,m,n);
    system("pause");
    return 0;
}