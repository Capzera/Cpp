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
#define mod 1000000007
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
void read(T &a) {
    cin >> a;
}
template <class T>
void read(T &a, T &b) {
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
void print(T n) {
    cout << n << endl;
}
template <class T>
void print(T m, T n) {
    cout << m << " " << n << endl;
}
template <class T>
void print(vector<T> num) {
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
int di[] = {-2, -1, 1, 2};
int solve() {
    string st = "*12345678", ed = "*87654321";
    unordered_set<string> s = {st};
    queue<string> q;
    q.push(st);
    int depth = 0;
    while (q.size()) {
        int size = q.size();
        while (size--) {
            string top = q.front();
            int pos = top.find('*');
            q.pop();
            if (top == ed) {
                return depth;
            }
            for (int d = 0; d < 4; d++) {
                int nPos = (pos + di[d] + 9) % 9;
                swap(top[pos], top[nPos]);
                if (!s.count(top)) {
                    q.push(top);
                    s.insert(top);
                }
                
                swap(top[pos], top[nPos]);
            }
        }
        depth++;
    }
    return -1;
}

int main() {
    cin.tie(nullptr)->sync_with_stdio(0);
    int t = 1;
    //cin >> t;
    while (t--){
        
        print(solve());
    }
    system("pause");
    return 0;
}
/*
*12345678
*87654321
*/