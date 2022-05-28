#include<bits/stdc++.h>

using namespace std;

int m,n;

int ans=0;

int dx[]={-2,-2,-1,-1,1,1,2,2};

int dy[]={1,-1,2,-2,-2,2,-1,1};

bool grid[10][10];

void dfs(int x,int y,int cur) {
    if (cur==m*n) {
        ans++;
        return;
    }
    grid[x][y]=1;
    for (int i=0;i<8;i++) {
        int xx=x+dx[i];
        int yy=y+dy[i];
        if (xx<0||xx>=m||yy<0||yy>=n) continue;
        if (grid[xx][yy]) continue;
        dfs(xx,yy,cur+1);
    }
    grid[x][y]=0;
}

int main(){
    cin>>m>>n;
    int x,y;
    cin>>x>>y;
    dfs(x,y,1);
    cout<<ans<<endl;
    system("pause");
    return 0;
}