#include<bits/stdc++.h> 
using namespace std;
int main()
{
	int m,n=1,t=1;
	cin>>m;
	for(int i=2;i<=m;i=i)
	{
		int j=0;
		t=t+1;
		while(j!=t)
		{
			n=n+t;
			j++;
			i++;
			if(i>m) break;
		}	
	}
	cout<<n;
	return 0;
}
