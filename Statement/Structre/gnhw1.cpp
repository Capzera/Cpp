#include<stdio.h>
#include<stdlib.h>
int main()
{
    struct Stu{
        int num;
        double avg;
    };
	int i,j;
    double a,b,c;
	Stu student[5];
	Stu temp;
    for(i=0;i<5;i++){
        scanf("%d %lf %lf %lf",&student[i].num,&a,&b,&c);
        student[i].avg=a+b+c/3;
    }
	for(i=0;i<5;i++){
		for(j=i+1;j<5;j++){
			if(student[i].avg<student[j].avg)
			{
				temp=student[i];
				student[i]=student[j];
				student[j]=temp;
			}
		}
	}
	for(i=0;i<5;i++)
	{
		printf("The num of Rank %d:%d\n",i+1,student[i].num);
	}
    system("pause");
	return 0;
}