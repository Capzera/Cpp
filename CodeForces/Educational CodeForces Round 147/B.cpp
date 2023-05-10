#pragma GCC optimize(2)
#include<bits/stdc++.h>
using namespace std;
#define vi vector<int>
#define inf 0x3f3f3f3f
template <class T>
void print(T &m, T &n) {
    cout << m << " " << n << endl;
}
void solve() {
    int n;
    cin >> n;
    vi numa(n + 1), numb(n + 1);
    numa[0] = numb[0] = inf;
    for (int i = 1; i <= n; i++) cin >> numa[i];
    for (int i = 1; i <= n; i++) cin >> numb[i];
    int l = 1, r = n;
    while (l < r && numa[l] == numb[l]) l++;
    while (l > 0 && numb[l] >= numb[l - 1]) l--;
    while (r > l && numa[r] == numb[r]) r--;
    while (r < n && numb[r] <= numb[r + 1]) r++;
    print(l, r);
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