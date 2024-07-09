#include<bits/stdc++.h>
using namespace std;
int main()
{
	freopen("energy.in","r",stdin);
	freopen("energy.out","w",stdout);
	long long n,m,c=0;
	long long a[100000]={},b[100000]={};
	cin>>n>>m;
	for(int i=0;i<n;i++)
	{
		cin>>a[i];
	}
	for(int i=0;i<n;i++)
	{
		cin>>b[i];
	}
	for(int i=0;i<(n+m)/2;i++)
	{
		if(a[i]<b[i])
		{
			c++;
		}
	}
	cout<<c;
	return 0;
}
