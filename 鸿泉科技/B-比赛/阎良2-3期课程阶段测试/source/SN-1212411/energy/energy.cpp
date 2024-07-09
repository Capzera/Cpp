#include<bits/stdc++.h> 
using namespace std;
int main()
{
	freopen("energy.in"," r",stdin);
	freopen("energy.out"," w",stdout);
	long long n,m,a[2000005],b[2000005],x=0;
	cin>>n>>m;
	for(int i=0;i<n;i++)
	{
		cin>>a[i];
	}
	for(int j=0;j<m;j++)
	{
		cin>>b[j];
	}
	for(int i=0;i<n;i++)
	{
		for(int j=0;j<m;j++)
		{
			if(a[i]>b[i])
			{
				x++;
			}
		}
	}
	cout<<x;
	return 0;
}
