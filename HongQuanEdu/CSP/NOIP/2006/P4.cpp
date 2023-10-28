#include <bits/stdc++.h>
using namespace std;
int main() {
    int k, n, base;
    long long ans = 0;
    vector<int> nums;
    cin >> k >> n;
    while (n) {
        nums.push_back(n % 2);
        n /= 2;
    }
    n = nums.size() - 1;
    for (; n >= 0; n--) {
        if (!nums[n]) continue;
        ans += pow(k, n);
    }
    cout << ans << endl;
    system("pause");
    return 0;
}