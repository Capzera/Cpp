#include<stdio.h>
#include<stdlib.h>
int main()
{  
    struct information{
        char name[255];
        char author[255];
        char publisher[255];
        double price;
    };
    information book[4];
    int i;
    for(i=1;i<=3;i++){
        scanf("%s %s %s",book[i].name,book[i].author,book[i].publisher);
        scanf("%lf",&book[i].price);
    }
    printf("BookName Author Publisher Price\n");
    for(i=1;i<=3;i++){
        printf("%8s",book[i].name);
        printf("%7s",book[i].author);
        printf("%10s",book[i].publisher);
        printf("%   .2lf\n",book[i].price);
    }
    system("pause");
	return 0;
}