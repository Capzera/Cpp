#include<bits/stdc++.h>
using namespace std;
int main()
{
	freopen("truck.in","r",stdin);
	freopen("truck.out","w",stdout);
	long long a,b,x,k,c=0,s=0;
	cin>>a>>b>>x>>k;
	for(int i=0;i<=a;i++)
	{
		c+=x;
		s++;
		if(s==k)
		{
			a++;
			b--;
		}
		s=0;
	}
	cout<<c<<endl;
	return 0;
}

