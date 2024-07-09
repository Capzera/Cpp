#include<bits/stdc++.h>
using namespace std;
int main()
{
	freopen("energy.in","r",stdin);
	freopen("energy.out","w",stdout);
	int n,m,s=0,k,ans=0;
	cin>>n>>m;
	int a[n],b[m];
	for(int i=0;i<n;i++)
	{
		cin>>a[i];
	} 
	for(int i=0;i<m;i++)
	{
		cin>>b[i];
	}
	sort(b+0,b+m);
	sort(a+0,a+n); 
	k=m;
	s=k;
	for(int i=0;i<m;i++)
	{
		k=s;
		for(int j=0;j<n;j++)
		{
			if(a[i]>b[k])
			{
				ans++;
				a[i]=0;
				break;
			}	
			k--;
		}
	}
	cout<<ans;
}
