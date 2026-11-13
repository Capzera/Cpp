#pragma GCC optimize(2)
#include<bits/stdc++.h>
using namespace std;
#define inf 0x3f3f3f3f
#define pii pair<int, int>
#define ll long long
#define vi vector<int>
#define vll vector<long long>
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
ll solve() {
    int n;
    ll ans = 0;
    cin >> n;
    vll nums(n);
    read(nums);
    if (n == 2) {
        ans = 1LL * nums[0] * nums[1];
        return ans;
    }
    vll pos, neg;
    for (auto& num : nums) {
        if (num > 0) pos.push_back(num);
        else neg.push_back(num);
    }
    sort(pos.begin(), pos.end(), greater<ll>());
    sort(neg.begin(), neg.end());
    int l1 = pos.size(), l2 = neg.size();
    if (l1 < 2 && l2 < 2) return ans;
    if (l1 > 1) {
        ans = 1LL * pos[0] * pos[1];
    }
    if (l2 > 1) {
        ll sum = 1LL * neg[0] * neg[1];
        if (sum > ans) ans = sum;
    }
    return ans;
}

int main() {
    int t = 1;
    
    cin >> t;
    while (t--){
        
        cout << solve() << endl;
    }
    system("pause");
    return 0;
}
/*

*/