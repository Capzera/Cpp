#include<bits/stdc++.h>
#include<conio.h>
#define TRUE 1
#define FALSE 0
#define MAXSIZE 100000
using namespace std;
typedef struct Node{
    int data;
    struct Node *next;
}Node,* LStack;
void InitStack(LStack *S)
{
    *S=(LStack)malloc(sizeof(Node));
    (*S)->next=NULL;
}
int PushStack(LStack S,int x)
{
    Node *p;
    p=(LStack)malloc(sizeof(Node)*MAXSIZE+1);
    if(p==NULL) return (FALSE);
    p->data=x;
    p->next=S->next;
    S->next=p;
    return (TRUE);
}
int PopStack(LStack S)
{
    Node *r;
    r=S->next;
    if(r==NULL) return (FALSE);
    S->next=r->next;
    free(r);
    return (TRUE);
}0
void CreateStack(LStack S)
{
    Node *p;
    int i,n,x;
    printf("请输入你想要在栈中预置几个元素:");
    scanf("%d",&n);
    system("cls");
    printf("请用空格隔开输出这%d个数:\n",n);
    for(i=1;i<=n;i++)
    {
        scanf("%d",&x);
        PushStack(S,x);
    }
    system("cls");
    printf("栈创建成功！\n\n\n\n\n");
    system("pause");
}
void PrintStack(LStack S)
{
    int i=0;
    Node *p;
    p=S->next;
    while (1==1)
    {
        i++;
        printf("%d ",p->data);
        p=p->next;
        if (p==NULL) break;
    }
    printf("\n");
}
int main(){
    LStack S;
    char ch1,ch2;
    int z=0,x;
    ch1='1';
    while(ch1!='5')
    {
        system("cls");
        printf("请选择你要使用的功能:\n");
        printf("1.初始化栈\n");
        printf("2.加入元素\n");
        printf("3.弹出元素\n");
        printf("4.打印栈\n");
        printf("5.退出\n");
        ch1=getch();
        switch(ch1)
        {
            case '1':
                system("cls");
                InitStack(&S);
                CreateStack(S);
                z=1;
            break;
            case '2':
                system("cls");
                if(!z){
                    printf("你还未创建栈!\n\n\n\n\n");
                    system("pause");
                    break;
                }
                printf("请输入你要压入栈的数值：\n");
                scanf("%d",&x);
                PushStack(S,x);
            break;
            case '3':
                system("cls");
                if(!z){
                    printf("你还未创建栈!\n\n\n\n\n");
                    system("pause");
                    break;
                }
                PopStack(S);
                printf("弹出成功！\n\n\n\n\n");
                system("pause");
            break;
            case '4':
                system("cls");
                if(!z){
                    printf("你还未创建栈!\n\n\n\n\n");
                    system("pause");
                    break;
                }
                PrintStack(S);
                printf("打印成功!!!\n\n");
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