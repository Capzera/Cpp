#include<iostream>
#include<stdio.h>
using namespace std;
int main()
{
	int i=3,x=1,y=1,n,cur,ans=1;
	cin>>n;
	if (n==1) cout<<1;
	else if (n==2) cout<<1;
	else
	{
		while (i<=n)
		{
			x=ans;
			ans+=y;
			y=x;
			i++;
		}
		cout<<ans%10007;
	}
}
