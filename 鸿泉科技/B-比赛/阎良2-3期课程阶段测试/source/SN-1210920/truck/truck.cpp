#include<bits/stdc++.h>
using namespace std;
long long a,b,x,k,s=0;
int main()
{
	freopen("truck.in","r",stdin);
	freopen("truck.out","w",stdout);
	cin>>a>>b>>x>>k;
	int i=0;
	while(a>=0)
	{
		if(i==k)
		{
			if(b!=0)
			{
			i=0;
			a++;
			b--;	
			}
		}
		i++;
		a=a-1;
		if(a>=0)
		{
		s+=x;	
		}
	}
	cout<<s;
	return 0;
} 
