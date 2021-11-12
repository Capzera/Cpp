#include<stdio.h>
#include<stdlib.h>
	struct Stu{
		int num;
		char name[255];
		int score1;
		int score2;
		double avg;
	};
void input(struct Stu student[]);
void output(struct Stu student[]);
int search(int n,struct Stu student[]);
int main(){
	struct Stu student[6];
	struct Stu temp;
	int i,j,n=5,nn,z=-1;
	input(student);
	output(student);
	for(i=1;i<5;i++)
		for(j=i+1;j<=5;j++)
			if(student[i].avg<student[j].avg){
				temp=student[i];
				student[i]=student[j];
				student[j]=temp;
			}
	output(student);
	printf("Press a number to search for information:");
    scanf("%d",&nn);
    i=search(nn,student);
    if(i>0){
	    printf("Num  Name  Score1  Score2  Average\n");
        printf("%4d %4s %7d %7d     %.2lf\n",student[i].num,student[i].name,student[i].score1,student[i].score2,student[i].avg);
    }
    else printf("This student has not found!");
	system("pause");
	return 0;
}
void input(struct Stu student[]){
	int i;
	for(i=1;i<=5;i++){
		scanf("%d %s %d %d",&student[i].num,student[i].name,&student[i].score1,&student[i].score2);
		student[i].avg=(student[i].score1+student[i].score2)/2.0;
	}
}
void output(struct Stu student[]){
	printf("Num  Name  Score1  Score2  Average\n");
	int i;
	for(i=1;i<=5;i++)printf("%4d %4s %7d %7d     %.2lf\n",student[i].num,student[i].name,student[i].score1,student[i].score2,student[i].avg);
}
int search(int n,struct Stu student[]){
    int i;
    for(i=1;i<=5;i++)
        if(student[i].num==n) return(i);
    return(0);
}