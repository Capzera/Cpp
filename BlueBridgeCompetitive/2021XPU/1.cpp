#include<bits/stdc++.h>
using namespace std;
int main()
{
	int a[31][31],ans[31][31],n,m,i,j,k,x,y;
	memset(ans,0,sizeof(ans));
	cin>>n>>m;
	for(i=1;i<=n;i++)
		for(j=1;j<=n;j++)
			cin>>a[i][j];
		for(i=1;i<=n;i++)
	for(k=1;k<=m-1;k++)
	{
		for(i=1;i<=n;i++)
			for(j=1;j<=n;j++)
				for(x=1;x<=n;x++) ans[i][j]+=a[i][x]*a[x][j];			
	}
	for(i=1;i<=n;i++)
	{
		for(j=1;j<=n;j++) cout<<ans[i][j]<<" ";
		cout<<endl;
	}
}
