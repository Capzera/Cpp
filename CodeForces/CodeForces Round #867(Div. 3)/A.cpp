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
    int n, t;
    cin >> n >> t;
    vi n1(n), n2(n);
    read(n1), read(n2);
    int maxe = 0, ans = -1;
    for (int i = 0; i < n; i++) {
        if (i + n1[i] > t) continue;
        if (n2[i] > maxe) {
            maxe = n2[i];
            ans = i + 1;
        }
    }
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
/*

*/