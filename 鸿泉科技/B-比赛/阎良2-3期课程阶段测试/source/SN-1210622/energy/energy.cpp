#include<bits/stdc++.h>
using namespace std;
long long cmp(long long x,long long y)
{
	return x > y;
}
int main()
{
	freopen("energy.in","r",stdin);
	freopen("energy.out","w",stdout);
	long long n,m;
	cin>>n>>m;
	long long a[n],b[m];
	long long ans=0;
	for(int i=0;i<n;i++)
	{
		cin>>a[i];
	}
	for(int i=0;i<m;i++)
	{
		cin>>b[i];
	}
	sort(a+0,a+n,cmp);
	sort(b+0,b+m,cmp);
	for(int j=0;j<m;j++)
	{
		for(int i=0;i<n;i++)
		{
			if(b[j]<=a[i])
			{
				ans++;
				j++; 
			}
		}
	}
	if(ans>=n) cout<<n;
	else cout<<ans;
	return 0;
}
