#include<bits/stdc++.h> 
using namespace std;
int main()
{
	freopen("energy.in","r",stdin);
	freopen("energy.out","w",stdout);
	long long n,m,c;
	cin>>n>>m;
	long long a[n];
	long long b[m];
	for(int i=0;i<n;i++)
	{
		cin>>a[i];
	}
	for(int i=0;i<m;i++)
	{
		cin>>b[m];
	}
	int w;
	if(n<m)
	{
		w=m;		
	}
	else
	{	
		w=n;
	}
	for(int i=0;i<w;i++)
	{
		for(int j=0;j<w;j++) 
		{
			if(a[i]>b[j+1])
			{
				c+=1;
			}
		}
	}
	cout<<c/6+1;
	return 0;
}
