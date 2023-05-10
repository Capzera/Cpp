#pragma GCC optimize(2)
#include<bits/stdc++.h>
using namespace std;
#define inf 0x3f3f3f3f
#define ll long long
#define vll vector<long long>
template <class T>
void read(vector<T> &a) {
    for (int i = 0; i < a.size(); i++) cin >> a[i];
}
template <class T>
void print(T &n) {
    cout << n << endl;
}
void solve() {
    ll n, k, sum = 0;
    cin >> n >> k;
    vll nums1(n), nums2(n);
    read(nums1); read(nums2);
    ll ans = inf, one = 0;
    sum = 0;
    for (int i = 0; i < n; i++) {
        ll size = nums2[i] - nums1[i] + 1;
        if (size == 1) one++;
        else sum += size;
        if (sum >= k) {
            ans = min(ans, nums2[i] - (sum - k) + 2 * (i + 1 - one));
            break;
        }
        if (sum + one >= k) {
            ans = min(ans, nums2[i] + 2 * (k - sum + i + 1 - one));
        }
    }
    ans = ans == inf ? -1 : ans;
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