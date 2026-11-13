#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#define LIST_INIT_SIZE 100
typedef struct ElementType
{
    int Num;
    char Name[20];
    double Price;
    int Rest;
}ElementType;
typedef struct list
{
    ElementType *element;
    int listsize;
    int length;
}SQList;
int Initlist(SQList &L)
{
    L.element = (ElementType *)malloc(LIST_INIT_SIZE * sizeof(ElementType));
    if (!L.element)
    {
        return -1;
    }
    L.length = 0;
    L.listsize = LIST_INIT_SIZE;
    return 1;
}
int Locatelist(SQList L,int x)
{
    int pos=-1;
    for (int i=0;i<=L.length;i++)
    {
        if (L.element[i].Num==x)
        {
            pos = i;
        } 
    }
    return pos;
}
int ListDelete(SQList &L,int i)
{
    if(i<1||i>L.length)return -1;
    struct ElementType *p,*q;
    p = &(L.element[i-1]);
    q = L.element+L.length-1;
    for(;p<=q;p++)
    {
        *p = *(p+1);
    }
    --L.length;
    return 0;
}
int main(){
    SQList List;
    int n,z=1,change;
    system("title 商品管理系统 By：/////////你的名字");
    system("color A1");
	while(1){
        system("cls");
        printf("欢迎使用商品管理系统，请按提示使用！\n");
		printf("1.输入信息\n2.阅览查询\n3.删除信息\n4.信息修改\n0.退出系统\n");
        char ch=getch();
        while (ch>52||ch<48) ch=getch();
        int sNum;
		if(ch=='0')
        {
            system("cls");
			printf("谢谢您的使用，再见！\n\n\n\n");
			break; 
		}
        if (ch=='1')
        {
            system("cls");
            if (!z)
            {
                system("cls");
                printf("数据已经导入，请勿重复操作！\n\n\n\n\n");
                system("pause");
            }
            else
            {
                Initlist(List);
                printf("请输入你想输入多少个商品的信息:");
                scanf("%d",&n);
                List.length=n;
                for (int i=0;i<n;i++)
                {
                    system("cls");
                    printf("请输入第%d个商品的信息:\n",i+1);
                    printf("编号：");
                    scanf("%d",&List.element[i].Num);
                    printf("名称：");
                    scanf("%s",List.element[i].Name);
                    printf("价格：");
                    scanf("%lf",&List.element[i].Price);
                    printf("剩余数量：");
                    scanf("%d",&List.element[i].Rest);
                    printf("\n");
                    z=0;
                }
                continue;
            }
        }
		if(ch=='2')
        {
            system("cls");
            if (z)
            {
                printf("您还未输入数据，请先进行输入操作!\n\n\n\n\n");
                system("pause");
            }
			else
            {
                printf("请输入你想查询的商品编号:");
                scanf("%d",&sNum);
                system("cls");
                if (Locatelist(List,sNum)<0)
                {
                    system("cls");
                    printf("该商品未查询到\n\n\n\n\n");
                    system("pause");
                } 
                else 
                {
                    printf("%d号商品的信息为:\n编号：%d\n名称：%s\n价格：%.2lf\n剩余数量：%d\n",sNum,List.element[Locatelist(List,sNum)].Num,List.element[Locatelist(List,sNum)].Name,List.element[Locatelist(List,sNum)].Price,List.element[Locatelist(List,sNum)].Rest);
                    system("pause");
                }
            }
            continue;
        }
		if(ch=='3')
        {
            system("cls");
            if (z)
            {
                printf("您还未输入数据，请先进行输入操作!\n\n\n\n\n");
                system("pause");
            }
            else
            {
                printf("请输入你想删除的商品编号:");
                scanf("%d",&sNum);
                system("cls");
                if (Locatelist(List,sNum)<0)
                {
                    system("cls");
                    printf("该商品未查询到!\n\n\n\n\n");
                    system("pause");
                }
                else {
                    ListDelete(List,Locatelist(List,sNum)+1);
                    printf("该商品删除成功！\n\n\n\n\n");
                    system("pause");
                }
            }
            continue;
        }
        if (ch=='4')
        {
            system("cls");
            if (z)
            {
                printf("您还未输入数据，请先进行输入操作!\n\n\n\n\n");
                system("pause");
            }
            else
            {
                printf("请输入你要修改商品的编号：");
                scanf("%d",&sNum);
                system("cls");
                if (Locatelist(List,sNum)<0)
                {
                    system("cls");
                    printf("该商品未查询到!\n\n\n\n\n");
                    system("pause");
                }
                else {
                    while(1)
                    {
                        printf("选择你要进行的操作：\n1.进货出货\n2.修改单价\n0.退出修改");
                        char ch2=getch();
                        while(ch2>50||ch2<48)ch2=getch();
                        system("cls");
                        if (ch2=='0')
                        {
                            break;
                        }
                        if (ch2=='1')
                        {
                            system("cls");
                            printf("1.进货\n2.出货\n0.返回菜单");
                            char ch3=getch();
                            while (ch3>50||ch3<49)ch3=getch();
                            system("cls");
                            if (ch3=='0'){
                                break;
                            }
                            if (ch3=='1')
                            {
                                printf("请输入要增加多少库存：");
                                scanf("%d",&change);
                                List.element[Locatelist(List,sNum)].Rest+=change;
                                system("cls");
                                printf("库存已增加!");
                            }
                            if (ch3=='2')
                            {
                                printf("请输入要减少多少库存：");
                                scanf("%d",&change);
                                system("cls");
                                List.element[Locatelist(List,sNum)].Rest-=change;
                                if (List.element[Locatelist(List,sNum)].Rest<0)
                                {   
                                    system("cls");
                                    printf("库存不允许为负数!\n\n\n\n\n");
                                    system("pause");
                                    system("cls");
                                    List.element[Locatelist(List,sNum)].Rest+=change;
                                    break;
                                }
                                printf("库存已减少！\n\n\n\n\n");
                            }
                        }
                        if (ch2=='2')
                        {
                            system("cls");
                            printf("请选择修改后的商品单价:");
                            scanf("%lf",&List.element[Locatelist(List,sNum)].Price);
                            printf("商品单价已修改\n");
                        }
                        system("pause");
                        system("cls");
                    }
                }              
            }
        }
    }
    system("pause");
    return 0;
}