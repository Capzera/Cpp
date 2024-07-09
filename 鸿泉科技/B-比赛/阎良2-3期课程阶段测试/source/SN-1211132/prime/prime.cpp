#include<bits/stdc++.h>
using namespace std;
int a[1000005];
bool zhishu(int z)
{
	if(z==1)
	{
		return false;
	}
	if(z==2||z==3)
	{
		return true;
	}
	if(z%6==1||z%6==5)
	{
		for(int i=2;i*i<=z;i++)
		{
			if(z%i==0)
			{
				return false;
			}
		}
		return true;
	}
	else
	{
		return false;
	}
}
int main()
{
	freopen("prime.in","r",stdin);
	freopen("prime.out","w",stdout);
	int n;
	cin>>n;
	for(int i=0;i<n;i++)
	{
		cin>>a[i];
		int j=0;
		while(1)
		{
			if(zhishu(a[i]-j))
			{
				cout<<a[i]-j<<' ';
				break;
			}
			if(zhishu(a[i]+j))
			{
				cout<<a[i]+j<<' ';
				break;
			}
			j++;
		}
	}
	return 0;
}
