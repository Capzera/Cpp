#include<bits/stdc++.h>
using namespace std;
int main() {
	int len;
	cin>>len;
	int half=len/2,j=0;
	if(len%2==0) {
		int MAX=9*pow(10,half-1);
		int i=pow(10,half-1);
		while (j<MAX){
			string s1=to_string(i),s2(s1);
			reverse(s2.begin(),s2.end());
			s1+=s2;
			cout<<j+1<<" "<<s1<<endl;
			j++;
			i+=pow(10,half-2);
		}
	}
	else {
		half++;
		int MAX=9*pow(10,half-1);
		int i=pow(10,half-1);
		while (j<MAX){
			string s1=to_string(i),s2;
			s2=s1.substr(0,half-1);
			reverse(s2.begin(),s2.end());
			s1+=s2;
			cout<<j+1<<" "<<s1<<endl;
			j++;
			i++;
		}
	}
	system("pause");
	return 0;
}
/*
返回值类型 函数名(传入参数1类型 传入参数1，传入参数2类型 传入参数2 ……){

}
1）有返回值类型函数
必须要有返回值
返回值类型必须和定义相匹配
返回值唯一

2) 无返回值类型函数——过程
必须不能有返回值
*/


//ProblemA:  键盘输入a,b，输出2*a+b*b;
//ProblemB:  键盘输入一个正整数n，求1-n的和
100 100 100 100 100 100 700
						1
						1
						1
						1
						1
						1
100  8
1	 7