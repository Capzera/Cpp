#include<bits/stdc++.h> 
using namespace std;
int main()
{
	freopen("truck.in","r",stdin);
	freopen("truck.out","w",stdout);
	long long a,x,b,k,n=0;
	cin>>a>>b>>x>>k;
	for(int i=1;i<=a;i++)
	{
		n+=x;
		for(int j=1;j<a;j++)
		{
			if(i==k*j)
			{
				b-=1;
				n+=x;
			}
		}
	} 
	cout<<n;
	return 0;
}
