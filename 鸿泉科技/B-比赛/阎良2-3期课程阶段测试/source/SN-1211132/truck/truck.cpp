//OK
#include<bits/stdc++.h>
using namespace std;
int main()
{
	freopen("truck.in","r",stdin);
	freopen("truck.out","w",stdout);
	long long a,b,x,k,h=0,g=0;
	cin>>a>>b>>x>>k;
	while(a)
	{
		a--;
		h++;
		if(h>=k&&b>0)
		{
			a++;
			b--;
			h=0;
		}
		g+=x;
	}
	cout<<g;
	return 0;
}
