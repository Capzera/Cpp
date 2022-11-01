#include<bits/stdc++.h>
using namespace std;
int n,m;
vector<vector<int>> grid;
vector<int> ans;
vector<bool> loc;
queue<int> q;
void bfs(queue<int> q,int depth) {
    int n=q.size();
    if (!n) return;
    while (n--) {
        int node=q.front();
        q.pop();
        ans[depth]++;
        for (auto i:grid[node]) {
            if (loc[i]) continue;
            q.push(i);
        }
    }
    bfs(q,depth+1);
}
int main(){
    cin>>n>>m;
    grid=vector<vector<int>> (n+1);
    ans=vector<int> (n+1);
    loc=vector<bool> (n+1);
    for (int i=0;i<m;i++) {
        int x,y;
        cin>>x>>y;
        grid[x].push_back(y);
    }
    q.push(1);
    bfs(q,0);
    for (int i=1;i<n;i++) cout<<ans[i]<<" ";
    system("pause");
    return 0;
}