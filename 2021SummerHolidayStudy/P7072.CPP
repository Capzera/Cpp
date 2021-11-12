#include<bits/stdc++.h>
using namespace std;
int t[605];
int n,w;
int main()
{
	int x;
	cin>>n>>w;
	for(int i=1;i<=n;i++)
	{
		cin>>x;
		t[x]++;
		int sum=0;
		for(int j=600;j>=0;j--)
		{
			sum+=t[j];
			if(sum>=max(1,i*w/100))
			{
				cout<<j<<' ';
				break;
			}
		}
	}
	system("pause");
	return 0;
} 