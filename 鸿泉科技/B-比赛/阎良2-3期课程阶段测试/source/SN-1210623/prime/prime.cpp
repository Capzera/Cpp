#include<bits/stdc++.h>
using namespace std;
bool zhi(long long x)
{
	if(x==1)
	{
		return 0;
	}
	if(x==2||x==3)
	{
		return 1;
	}
	if(x%6!=1&&x%6!=5)
	{
		return 0;
	}
	long long n=sqrt(x);
	for(int i=5;i<=n;i+=6)
	{
		if(x%i==0||x%(i+2)==0)
		{
			return 0;
		}
	}
	return 1;
}
int main()
{
	freopen("prime.in","r",stdin);
	freopen("prime.out","w",stdout);
	long long n;
	cin>>n;
	long long a[n];
	for(long long i=0;i<n;i++)
	{
		cin>>a[i];
	}
	for(long long i=0;i<n;i++)
	{	
		if(a[i]==1)
		{
			cout<<"2"<<" ";
	 	} 
		for(long long j=a[i];j>0;j--)
		{
			if(zhi(a[i])==1)
			{
				cout<<a[i]<<" ";
				break;
			}
			else
			{
				a[i]--;
			}
		}
	}
}
