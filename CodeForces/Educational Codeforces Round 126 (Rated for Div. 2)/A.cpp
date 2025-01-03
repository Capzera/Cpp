#pragma GCC optimize(2)
#include<bits/stdc++.h>
using namespace std;
#define inf 0x3f3f3f3f
#define pii pair<int, int>
#define ll long long
#define vi vector<int>
#define yn(ans) printf("%s\n", (ans)? "Yes" : "No")
#define YN(ans) printf("%s\n", (ans)? "YES" : "NO")
template <class T>
void read(vector<T> &a) {
    for (int i = 0; i < a.size(); i++) cin >> a[i];
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
template <class T>
T max(T &a, T &b, T &c) {
    return a > b && a > c ? a : (b > a && b > c ? b : c);
}
void solve() {
    int n;
    cin >> n;
    vi n1(n), n2(n);
    read(n1), read(n2);
    ll ans = 0;
    for (int i = 1; i < n; i++) {
        ans += min(abs(n1[i] - n1[i - 1]) + abs(n2[i] - n2[i - 1]), abs(n1[i] - n2[i - 1]) + abs(n2[i] - n1[i - 1]));
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