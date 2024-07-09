#include<bits/stdc++.h>
using namespace std;
int main()
{
	int n,a,k,b,m;
	cin>>n>>a>>k>>b;
	if(n%2==0)
	{
		n/=2;
		for(int i=1;1<=2;i++)
		{
			if(k*b<a*k)
			{
				int che=n/k*b;
				m+=che;
				int shen=n-k*b;
				if(shen!=0)
				{
					if(shen/k<shen*a)
						if(shen%k==0) m+=shen/k*b;
						else m+=(shen/k+1)*b;
					else m+=a*shen;
				}
			}
			if(k*b>a*k)
			{
				int tong=n/a;
				m+=tong;
				int shen=n-n/a;
				if(shen!=0)
				{
					if(shen/k*b>shen*a)
						m+=shen*a;
					else m+=(shen/k+1)*b;
				}
			}
		}
		cout<<m;
	}
	else cout<<"-1";
	return 0;
}
