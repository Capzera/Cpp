#include <bits/stdc++.h>
using namespace std;
    int dx[] = {-2, -1, 1, 2};
    int dy[] = {1, 2, 2, 1};
    int m, n, ans = 0;
    int dfs(int x, int y){
        if (x < 0 || x > m || y < 0 || y > n) return 0;
        if (x == m && y == n){
            return 1;
        }
        return dfs(x - 2, y + 1) + dfs (x - 1, y + 2) + dfs(x + 1, y + 2) + dfs(x + 2, y + 1);
    }

int main(){
    cin>>m>>n;
    ans = dfs(0, 0);
    cout<<"ans="<<ans<<endl;
    system("pause");
    return 0;
}
/*
0000100
0010001
1100010
0010000
*/