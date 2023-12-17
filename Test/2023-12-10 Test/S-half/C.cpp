#include <bits/stdc++.h>
using namespace std;
int main() {
    int n, T, x;
    cin >> n >> T;
    unordered_map<int, int> mp;
    vector<long long> ret(T + 1);
    long long ans = 0;
    for (int i = 0; i < n; i++) {
        cin >> x;
        mp[x]++;
    }
    for (int i = 1; i <= T; i++) {
        if (!mp.count(i)) continue;
        for (int j = i; j <= T; j += i) {
            ret[j] += mp[i];
        }
    }
    for (int i = 1; i <= T; i++) {
        ret[i] += ret[i - 1];
        ans ^= ret[i];
    }
    cout << ans << endl;
    return 0;
}