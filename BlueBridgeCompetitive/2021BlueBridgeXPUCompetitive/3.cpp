#include<iostream>
#include<stdio.h>
#include<string.h>
using namespace std;
int main()
{
	int a,i,temp;
	cin>>a;
	string s="",t="";
	while (a!=0)
	{
		temp=a%16;
		a/=16;
		if (temp<10) s+=temp+'0';
		else s+=temp+55;
	}
	for(i=0;i<s.length();i++) t+=s[s.length()-i-1];
	cout<<t;
}
