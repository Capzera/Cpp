#include<bits/stdc++.h>
using namespace std;
int main()
{
	freopen("truck.in","r",stdin);
	freopen("truck.out","w",stdout);
	long long a,b,x,k;
	cin>>a>>b>>x>>k;
	long long sum=0;
	if(b)
	{
		if(k==1)
		{
			sum=(a+b)*x;
		}
		else
		{
			sum=a/k;
			sum+=a;
			sum*=x;
		}
		
	}
	else
	{
		sum=a*x;
	}
	cout<<sum;
	return 0;
} 
