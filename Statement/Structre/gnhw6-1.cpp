#include<stdio.h>
#include<stdlib.h>
using namespace std;
struct data{
    int x,y;
};
int main(){
    struct data *p;
    struct data array[2]={{8,5},{9,3}};
    printf("%d\n",(array[0].x+array[0].y)/array[1].y);
    p=array;
    (p++)->y=p->y+10;
    p->x=p->x-5;
    printf("%d\n",array[0].y+array[1].x);
    system("pause");
    return 0;
}