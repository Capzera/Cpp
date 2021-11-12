#include <stdio.h>
#include <stdlib.h>
#include "module.h"
using namespace std;
int main(){
	SqQueue que;
	int e,a,b,i=0,j;
	int m[50];
	InitQueue(que);
	printf("输入一个十进制数："); 
	scanf("%d",&a);
	printf("转换成几进制？（输入数字）");
	scanf("%d",&b);
	do{
		e=a%b;
		a=a/b;
		EnQueue(que,e);
		i++;
	}while(a>0);
	j=i;
	do{
		DeQueue(que,e);
		m[i]=e;
	}while(--i);
	if(b==16){
		printf("0x");
		for(i=1;i<=j;i++){
			if(m[i]>=10) printf("%c",m[i]+55);
			else printf("%d",m[i]);
		}
	}
	else{
		if(b==8) printf("0");
		for(i=1;i<=j;i++){
		printf("%d",m[i]);
	}
	}
	system("pause");
	return 0;
}