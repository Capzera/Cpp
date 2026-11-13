#include <stdio.h>
#include<stdlib.h>
int main()
{
    int i = 1;
    int ret = (++i)+(++i)+(++i);
    printf("ret = %d\n", ret);
    system("pause");
	return 0;
}