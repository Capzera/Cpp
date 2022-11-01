#include<iostream>
#include<stdio.h>
#include<string.h>
using namespace std;
int main()
{
	string s,compare;
	cin>>s;
	int i,j,ptr1,len1=s.length(),ans=len1,len2,sen=0;
	for(i=1;i<=len1;i++)
	{
		if (len1%i!=0) continue;
		compare="";sen=0;
		for(j=0;j<i;j++) compare+=s[j];
		len2=compare.length();
		ptr1=0;
		for(j=0;j<len1;j++)
		{
			if (s[j]!=compare[ptr1])
			{
				sen=1;
		 		break;
			}
			ptr1++;
			if (ptr1==len2) ptr1=0;
		}
		if (ptr1==0&&sen==0)
		{
			cout<<i<<endl;
			break;	
		}
	} 

}
