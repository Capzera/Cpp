#include<bits/stdc++.h>
using namespace std;
int main()
{
	freopen("string.in","r",stdin);
	freopen("string.out","w",stdout);
	string s,s1;
	cin>>s;
	cin>>s1;
	int a=0,b=0;
	for(int i=0;i<s.size();i++)
	{
		if(s[i]=='1')
		{
			a++;
		}
	}
	for(int i=0;i<s1.size();i++)
	{
		if(s1[i]=='1')
		{
			b++;
		}
	}
	if(b==1&&a==1)
	{
		cout<<"10";
	}
}
