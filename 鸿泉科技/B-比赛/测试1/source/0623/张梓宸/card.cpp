#include<bits/stdc++.h>
using namespace std;
int main()
{
	int n,m,s=0;
	cin>>n>>m;
	int a[n],b[m];
	for(int i=0;i<n;i++) cin>>a[i];
	for(int i=0;i<m;i++) cin>>b[i];
	sort(a,a+n,greater<int>());
	sort(b,b+m,greater<int>());
	for(int i=0;i<m;i++)
	{
		for(int j=0;j<m;j++)
			if(a[i]>b[j])
			{
				s++;
				a[i]=b[j]=10000;
				break;
			} 
	}
	cout<<s;
	return 0;
} 
