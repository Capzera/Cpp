#include<bits/stdc++.h>
using namespace std;
bool cmp(int A,int B)
{
	return A>B;
}
int main()
{
	freopen("energy.in","r",stdin);
	freopen("energy.out","w",stdout);
	int n,m;
	vector<int> a;
	vector<int> b;
	cin>>n>>m;
	for(int i=0;i<n;i++)
	{
		int c;
		cin>>c;
		a.push_back(c);
	}
	sort(a.begin(),a.end(),cmp);
	for(int i=0;i<m;i++)
	{
		int c;
		cin>>c;
		b.push_back(c);
	}
	sort(b.begin(),b.end(),cmp);
	if(a.back()>b[0])
	{
		cout<<(m>n?n:m);
		return 0;
	}
	if(b.back()>a[0])
	{
		cout<<0;
		return 0;
	}
	int g=0;
	for(int i=n-1;i>=0;i--)
	{
		for(int j=m-1;j>=0;j--)
		{
			if(a[i]>b[j]&&b[j]!=-1)
			{
				g++;
				b[j]=-1;
				a[i]=-1;
			}
		}
	}
	cout<<g;
	return 0;
}
