#include<bits/stdc++.h>
using namespace std;
int a[100][100]={0},num=0;
int x[9]={0,1,2,2,1,-1,-2,-2,-1},y[9]={0,-2,-1,1,2,2,1,-1,-2},n;
bool b[100][100]={0};
void dfs(int,int,int);
int main(){
	cin>>n;
	a[1][1]=1;b[1][1]=1;
	dfs(1,1,2);
	cout<<num<<endl;
	system("pause");
}
void dfs(int l,int r,int t){
	int i,j,x1,y1;
	if (t>n*n){
		num++;
		if(num<=2){
			for(i=1;i<=n;i++){
				for(j=1;j<=n;j++)cout<<setw(5)<<a[i][j];
				cout<<endl;
			}
			cout<<endl;
		}
		return;
	}
	for(int i=1;i<=8;i++){
		x1=l+x[i];y1=r+y[i];
		if (x1<=n&&x1>=1&&y1<=n&&y1>=1&&!b[x1][y1]){
			b[x1][y1]=1;
			a[x1][y1]=t;
			dfs(x1,y1,t+1);
			b[x1][y1]=0;
		}
	}
}
