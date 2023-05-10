#include<iostream>
#include<stdio.h>
#include<string.h>
using namespace std;
int main()
{
	string s1,s2,ans="",t="";
	cin>>s1;
	cin>>s2;
	int i,j,count,temp=0,num1[101],num2[101],len1=s1.length(),len2=s2.length();
	if (len1<len2) 
	{
		swap(len1,len2);
		swap(s1,s2);
	}
	for(i=len1-1;i>=0;i--) num1[len1-i]=s1[i]-'0';
	for(i=len2-1;i>=0;i--) num2[len2-i]=s2[i]-'0';
	for(i=1;i<=len1;i++)
	{
		if (i>len2) num2[i]=0;
		count=num1[i]+num2[i]+temp;
		if (count<=9) 
		{
			t+=count+'0';
			temp=0;
		}
		else
		{
			t+=count-10+'0';
			temp=1;
		}
	}
	if (temp) t+='1';
	for(j=0;j<t.length();j++) ans+=t[t.length()-j-1];
	cout<<ans<<endl;
}
