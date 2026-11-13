#include <bits/stdc++.h>
#define ll long long
using namespace std;
int main(){
    cin.tie(nullptr)->sync_with_stdio(false);
    int m, n;
    ll ans = 0;
    cin >> m >> n;
    vector<string> grid(m);
    for (int i = 0; i < m; i++) cin >> grid[i];
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            char target = grid[i][j];
            for (int len = 1; len <= min(i, j); len++) {//左上
                if (grid[i - len][j] != target || grid[i][j - len] != target) break;
                int x = i, y = j - len;
                bool flag = 1;
                for (int dia = 0; dia < len - 1; dia++) {
                    if (grid[--x][++y] != target) {
                        flag = 0;
                        break;
                    }
                }
                if (flag) ans++;
            }
            for (int len = 1; len <= min(i, n - j - 1); len++) {//右上
                if (grid[i - len][j] != target || grid[i][j + len] != target) break;
                int x = i, y = j + len;
                bool flag = 1;
                for (int dia = 0; dia < len - 1; dia++) {
                    if (grid[--x][--y] != target) {
                        flag = 0;
                        break;
                    }
                }
                if (flag) ans++;
            }
            for (int len = 1; len <= min(m - i - 1, j); len++) {//左下
                if (grid[i + len][j] != target || grid[i][j - len] != target) break;
                int x = i + len, y = j;
                bool flag = 1;
                for (int dia = 0; dia < len - 1; dia++) {
                    if (grid[--x][--y] != target) {
                        flag = 0;
                        break;
                    }
                }
                if (flag) ans++;
            }
            for (int len = 1; len <= min(m - i - 1, n - j - 1); len++) {//右下
                if (grid[i + len][j] != target || grid[i][j + len] != target) break;
                int x = i + len, y = j;
                bool flag = 1;
                for (int dia = 0; dia < len - 1; dia++) {
                    if (grid[--x][++y] != target) {
                        flag = 0;
                        break;
                    }
                }
                if (flag) ans++;
            }
        }
    }
    cout << ans << endl;
    return 0;
}
