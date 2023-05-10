#include<iostream>
#include<stdio.h>
#include<string.h>
using namespace std;
int isalpha(char);
int main()
{
	char s[10001];
	gets(s);
	int len=strlen(s),i,zt1=0,ans=0;
	for(i=0;i<len;i++)
	{
		if (s[i]>=65&&s[i]<=90) s[i]+=32;
		if (isalpha(s[i])==1&&zt1==0) zt1=1;
		if (zt1==1&&isalpha(s[i])==0)
		{
			zt1=0;
			ans++;
		}
	}
	if (zt1) ans++;
	cout<<ans<<endl;
}
int isalpha(char c)
{
	if (c>=97&&c<=122) return 1;
	else return 0;
}
