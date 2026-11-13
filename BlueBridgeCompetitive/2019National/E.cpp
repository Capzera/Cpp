#pragma GCC optimize(2)
#include <bits/stdc++.h>
using namespace std;
#define inf 0x3f3f3f3f
#define pii pair<int, int>
#define pll pair<long long, long long>
#define umii unordered_map<int, int>
#define umll unordered_map<long long, long long>
#define umsi unordered_map<string, int>
#define ll long long
#define vi vector<int>
#define vll vector<long long>
#define vvi vector<vector<int>>
#define vvl vector<vector<long long>>
#define vs vector<string>
#define yn(ans) printf("%s\n", (ans)? "Yes" : "No")
#define YN(ans) printf("%s\n", (ans)? "YES" : "NO")
int dx[] = {0, 0, 1, -1, -1, -1, 1, 1}, dy[] = {1, -1, 0, 0, -1, 1, -1, 1};
template <class T>
void read(vector<T> &a) {
    for (int i = 0; i < a.size(); i++) cin >> a[i];
}
template <class T>
void read(int &a) {
    cin >> a;
}
template <class T>
void read(int &a, int &b) {
    cin >> a >> b;
}
template <class T>
void read(vector<vector<T>> &a) {
    int m = a.size();
    if (!m) return;
    int n = a[0].size();
    for (int i = 0; i < m; i++)
        for (int j = 0; j < n; j++) cin >> a[i][j];
}
template <class T>
void print(T &n) {
    cout << n << endl;
}
template <class T>
void print(T &m, T &n) {
    cout << m << " " << n << endl;
}
template <class T>
void print(vector<T>& num) {
    for (auto& x : num) cout << x << " ";
    cout << endl;
}
template <class T>
void print(vector<vector<T>> grid) {
    for (auto& i : grid) {
        for (auto& j : i) cout << j << " ";
        cout << endl;
    }
}
int MOD (int x, int y) {
    return x - y * (x / y);
}
ll Abs(ll a) {
    ll b = a >> 31;
    return (a + b) ^ b;
}
template <class T>
T max(T &a, T &b) {
    return a > b ? a : b;
}
template <class T>
T max(T &a, T &b, T &c) {
    return a > b && a > c ? a : (b > a && b > c ? b : c);
}
void solve() {
    int n = 5;
    vvi grid(n + 1, vi(n + 1));
    ll ans = 0;
    umii mp = {
        {0, 1},
        {1, 0},
        {2, 3},
        {3, 2}
    };
    function<void(int, int, int, vvi)> dfs = [&](int x, int y, int depth, vvi grid) {
        if (depth > 12) return;
        if (depth > 3 && !x && !y) {
            ans++;
            return;
        }
        for (int d = 0; d < 4; d++) {
            int mx = x + dx[d], my = y + dy[d];
            if (mx < 0 || mx > n || my < 0 || my > n || grid[mx][my]) continue;
            grid[mx][my] = 1;
            dfs(mx, my, depth + 1, grid);
            grid[mx][my] = 0;
        }
    };
    dfs(0, 0, 0, grid);
    print(ans);
}

int main() {
    cin.tie(nullptr)->sync_with_stdio(0);
    clock_t st, ed;
    st = clock();
    int t = 1;
    
    //cin >> t;
    while (t--){
        
        solve();
    }
    ed = clock();
    double dur = (double) (ed - st) / CLOCKS_PER_SEC;
    //cout << "Running is during " << dur << " seconds" << endl;
    system("pause");
    return 0;
}
/*

*/