#include<iostream>
#include<cmath>
#include<Windows.h>
#include<stdlib.h>
using namespace std;
//关键公式（x^2+y^2-1)^3-x^2*y^3=0,小于零的时候在心形内部，大于零的时候在外部 
void big_heart(){//大爱心的打印，一行一行打印爱心
	double x_1,y_1,m_1,n_1;
	for(y_1=1.5;y_1>-1.5;y_1-=0.1){ //心型的最大长度 
		for(x_1=-1.5;x_1<=1.5;x_1+=0.05){//心型的最大宽度 
		m_1=x_1*x_1+y_1*y_1-1;
		n_1=pow(m_1,3)-pow(x_1,2)*pow(y_1,3);
		n_1<0? cout<<'x': cout<<' ';
	}
	Sleep(100);
	cout<<endl;
}
}
void big_heart_2(){ //大爱心的打印，迅速的完成爱心的打印
	double x_1,y_1,m_1,n_1;
	for(y_1=1.5;y_1>-1.5;y_1-=0.1){ //心型的最大长度 
		for(x_1=-1.5;x_1<=1.5;x_1+=0.05){//心型的最大宽度 
		m_1=x_1*x_1+y_1*y_1-1;
		n_1=pow(m_1,3)-pow(x_1,2)*pow(y_1,3);
		n_1<0? cout<<'x': cout<<' ';
	}
	cout<<endl;
}
}
void small_heart(){ //小爱心的打印
	double x,y,m,n;
	for(y=3.0;y>-3.0;y-=0.2){ //心型的最大长度 
		for(x=-3.0;x<=3.0;x+=0.1){//心型的最大宽度 
		m=x*x+y*y-1;
		n=pow(m,3)-pow(x,2)*pow(y,3);
		n<0? cout<<'x': cout<<' ';
	}
	cout<<endl;
 }
}
//为了美观和舒适，隐藏打印时的光标
HANDLE han = GetStdHandle(-11);
void hide(){
	CONSOLE_CURSOR_INFO cursor;
	cursor.bVisible = 0;
	cursor.dwSize = 1;
	SetConsoleCursorInfo(han,&cursor);
}
int main(){  //主函数入口
	hide(); 
	system("color C"); //颜色调节
	big_heart();
	int a=1;
while(1){
	system("cls");
	if(a%2==0){
		small_heart();
		Sleep(400); //控制停顿时间
	}else{
		big_heart_2(); 
		Sleep(100); //控制停顿时间
		}
	a++;
}
    system("pause");
	return 0;
}