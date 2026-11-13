#pragma GCC optimize(2)
#include<bits/stdc++.h>
using namespace std;
#define inf 0x3f3f3f3f
#define ll long long
#define vi vector<int>
template <class T>
void print(T &n) {
    cout << n << endl;
}
void solve() {
    int n, k, c, x, y;
    cin >> n >> k >> c;
    vi edge[n + 1];
    for (int i = 1; i < n; i++) {
        cin >> x >>  y;
        edge[x].push_back(y);
        edge[y].push_back(x);
    }
    ll ans = 0;
    vi dp1(n + 1), dp2(n + 1), dp3(n + 1);
    function<void(int, int, vi &)> dfs = [&](int sn, int fa, vi &dp) -> void{
        for (auto &fn : edge[sn]) {
            if (fn == fa) continue;
            dp[fn] = dp[sn] + 1;
            dfs(fn, sn, dp);
        }
    };
    int u = 1;
    dfs(u, 0, dp1);
    for (int i = 1; i <= n; i++)
        if (dp1[i] > dp1[u]) u = i;
    dfs(u, 0, dp2);
    for (int i = 1; i <= n; i++)
        if (dp2[i] > dp2[u]) u = i;
    dfs(u, 0, dp3);
    for (int i = 1; i <= n; i++) {
        ans = max(ans, 1LL * k * max(dp2[i], dp3[i]) - 1LL * c * dp1[i]);
    }
    print(ans);
}

int main() {
    int t = 1;
    cin >> t;
    while (t--){
        solve();
    }
    system("pause");
    return 0;
}
/*

*/
/*#pragma GCC optimize(2)
#include<bits/stdc++.h>
using namespace std;
#define inf 0x3f3f3f3f
#define pii pair<int, int>
#define ll long long
#define vi vector<int>
#define yn(ans) printf("%s\n", (ans)? "Yes" : "No")
#define YN(ans) printf("%s\n", (ans)? "YES" : "NO")
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
template <class T>
T max(T &a, T &b, T &c) {
    return a > b && a > c ? a : (b > a && b > c ? b : c);
}
template <class T>
T max(T &a, T &b) {
    return a > b ? a : b;
}
void solve() {
    int n, k, c, x, y;
    cin >> n >> k >> c;
    vector<int> edge[n + 1];
    for (int i = 1; i < n; i++) {
        cin >> x >>  y;
        edge[x].push_back(y);
        edge[y].push_back(x);
    }
    ll ans = 0;
    vi dp1(n + 1), dp2(n + 1), dp3(n + 1);
    function<void(int, int)> dfs = [&](int sn, int fa, vi &dp) -> void{
        for (auto &fn : edge[sn]) {
            if (fn == fa) continue;
            dp[fn] = dp[sn] + 1;
        }
    };
    dfs1(1, 0, dp1);
    for (auto& i : dp) {
        ll sum = 1LL * i * k;
        if (sum > ans) ans = sum;
    }
    function<void(int, int, int)> dfs2 = [&](int sn, int fa, int diff) -> void {
        for (auto &fn : edge[sn]) {
            if (fn == fa) continue;
            //dp[sn] += diff;
            if (dp[fn] + 1 == dp[sn]) 
            int maxdepth = max(dp[fn], dp[sn] + diff);
            ll score = 1LL * maxdepth * k - 1LL * diff * c;
            if (score > ans) ans = score;
            dfs2(fn, sn, diff + 1);
        }
    };
    dfs2(1, 0, 1);
    cout << ans << endl;
}

int main() {
    int t = 1;
    
    cin >> t;
    while (t--){
        
        solve();
    }
    system("pause");
    return 0;
}
*/