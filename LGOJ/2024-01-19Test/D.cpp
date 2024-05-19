#include <bits/stdc++.h>
using namespace std;
unordered_map<long long, int> mp;
long long sum, cnt, ans;
int main() {
    int n, x;
    cin >> n;
    mp[0] = 1;
    while (n--) {
        cin >> x;
        sum += x & 1 ? 1 : -1;
        cnt += x & 1 ? mp[sum - 1] : -mp[sum];
        ans += cnt;
        mp[sum]++;
    }
    cout << ans;
    return 0;
}
/*
1 3 2 4 5 6 7
1 2 1 0 1 0 1
1 2 1
*/