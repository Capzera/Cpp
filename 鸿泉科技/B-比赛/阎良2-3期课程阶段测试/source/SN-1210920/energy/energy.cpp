#include<bits/stdc++.h>
using namespace std;
long long n,m,o=0,s=0,u=0,k=0; 
int main()
{
	freopen("energy.in","r",stdin);
    freopen("energy.out","w",stdout);
	cin>>n>>m;
	long long a[n],b[m];
	for(int i=0;i<n;i++)
	{
		a[i]=0;
		cin>>a[i];
	}
	for(int i=0;i<m;i++)
	{
		b[i]=0;
		cin>>b[i];
	}
	for(int i=0;i<n;i++)
	{
		for(int j=0;j<m;j++)
		{
			if(a[i]>b[j]&&s<b[j])
			{
				s=b[j];
				o=j;
				u=1;
			}	 
		}
		if(u==1)
		{
		b[o]=0;	
		k++;
		}
		o=0;
		u=0;
		s=0;
	}
	cout<<k;
   return 0;
}
