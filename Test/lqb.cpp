#include<bits/stdc++.h>
using namespace std;
int n,m,ans=INT_MAX;
int dx[4]={1,0,-1,0},dy[4]={0,1,0,-1};
char a[301][301];
bool check[301][301]={0};
void dfs(int i,int j,int count)
{
    int x,y,k;
    if ((i==n-1)&&(j==m-1)) 
    {
        ans=min(ans,count);
        return;
    }
    check[i][j]=true;
    for(k=0;k<4;k++)
    {
        x=i+dx[k];y=j+dy[k];
        if (check[x][y]==true) continue;
        if (x==-1||x==n||y==-1||y==m) continue;
        if (a[i][j]+a[x][y]=='d') continue;
        dfs(x,y,count+int((a[x][y]-'0')/2));
    }
    check[i][j]=false;
}
int main()
{
    cin>>n>>m;
    for(int i=0;i<n;i++) cin>>a[i];
    dfs(0,0,0);
    if (ans==INT_MAX) cout<<-1<<endl;
    else cout<<ans<<endl;
    system("pause");
    return 0;
}