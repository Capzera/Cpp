#include<bits/stdc++.h>
using namespace std;
int main()
{
	freopen("truck.in","r",stdin);
	freopen("truck.out","w",stdout);
	long long a,b,x,k,m=0,ans=0;
	cin>>a>>b>>x>>k;
	for(long long i=0;i<a+b;i++)
	{
		if(a>0)
		{
			m++;
			ans+=x;
			a--; 
		}
		if(m==k)
		{
			b--;
			a++;
			m=0;
		}
	}
	cout<<ans;	
} 
