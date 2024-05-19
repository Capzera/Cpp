#include <bits/stdc++.h>
using namespace std;
int main() {
    int t, n, ans;
    cin >> t;
    while (t--) {
        cin >> n;
        vector<int> nums(n);
        ans = 0;
        for (auto& x : nums) cin >> x;
        for (int k = 1; k <= n; k++) {
            int gcd = 0;
            if (n % k) continue;
            for (int i = 0; i + k < n; i++) {
                gcd = __gcd(gcd, abs(nums[i] - nums[i + k]));
            }
            ans += gcd != 1;
        }
        cout << ans << endl;
    }
    system("pause");
    return 0;
}
