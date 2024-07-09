#include<bits/stdc++.h> 
using namespace std;
long long n,n1[5000005],l=1;
int main()
{
	freopen("prime.in"," r",stdin);
	freopen("prime.out"," w",stdout);
	cin>>n;
	for(int i=0;i<n;i++)
	{
		cin>>n1[i];
		
	
		for(int f=2;f==n1[i];f++)
		{
			if(n1[i]%f==0)
			{
				cout<<n1[i];
			}
		}
		n1[i]-=l;
	}
	return 0;
	}

