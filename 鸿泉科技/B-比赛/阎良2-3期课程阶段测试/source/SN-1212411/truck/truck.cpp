#include<bits/stdc++.h> 
using namespace std;
int main()
{
	freopen("truck.in","r",stdin);
	freopen("truck.out","w",stdout);
	int a,b,x,k,jg=0,c=0;
	cin>>a>>b>>x>>k;
	jg=a*x;
	c=a/k;
	if(c>b)
	{
		jg+=b*x;
		cout<<jg;
	return 0;
	}
	jg+=c*x;
	cout<<jg;
	return 0;
}
