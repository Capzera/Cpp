#include<bits/stdc++.h>
using namespace std;
int main(){
    
    system("pause");
    return 0;
}

    : 

#include<bits/stdc++.h>
#define MAXSIZE 10000
#define ERROR 0
#define OK 1
using namespace std;
typedef struct 
{
    int data[MAXSIZE+1];
    int length;
}List;
void InitList(List *L)
{
    L=(List *)malloc(sizeof(List));
    L->length=0;
}
void CreateList(List *L,int a[],int n)
{
    int i;
    for(i=1;i<=n;i++){
        L->data[i]=a[i];
        L->length++;
    }
}
int InsList(List *L,int i,int x)
{
    int *p,*q;
    if ((i<1) || (i>L->length+1) || (L->length>=MAXSIZE)) return ERROR;
    p=L->data+i;
    q=L->data+L->length+1;
    for(;q>p;q--) *q=*(q-1);
    *p=x;
    L->length++;
    return OK;
}
int DelList(List *L,int i,int *e)
{
    int *p,*q;
    if ((i<1) || (i>L->length)) return ERROR;
    p=L->data+i;
    q=L->data+L->length;
    for(;p<=q;p++)*p=*(p+1);
    L->length--;
    return OK;
}
void PrintList(List *L)
{
    int i;
    for(i=1;i<=L->length;i++)printf("%d ",L->data[i]);
    printf("\n");
}
int main()
{
    int n,i,a[10001];
    List *L;
    scanf("%d",&n);
    for(i=1;i<=n;i++)scanf("%d",&a[i]);
    InitList(L);
    CreateList(L,a,n);
    PrintList(L);
    system("pause");
    return 0;
}