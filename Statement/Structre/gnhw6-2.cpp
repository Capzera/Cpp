#include<stdio.h>
#include<stdlib.h>
using namespace std;
struct abc{
    char c;
    double v;
};
void fun(struct abc *b){
    (*b).c='A';
    (*b).v=80.7;
}
int main(){
    struct abc a={'B',98.5};
    fun(&a);
    printf("%c %4.1f\n",a.c,a.v);
    system("pause");
    return 0;
}