#include <bits/stdc++.h>
using namespace std;
int main() {
    long long n, x, y;
    long long ans = LLONG_MIN;
    cin >> n >> x >> y;
    vector<long long> nums(n);
    for (int i = 0; i < n; i++) cin >> nums[i];
    sort(nums.begin(), nums.end());
    for (int i = 0; i <= min(n, x); i++) {//删掉末尾的i个
        long long sum = 0;
        long long j = n - i - 1;
        for (int k = 0; j >= 0 && k < y; k++) {
            sum -= nums[j];
            j--;
        }
        for (; j >= 0; j--) {
            sum += nums[j];
        }
        ans = max(ans, sum);
    }
    printf("%lld\n", ans);
    system("pause");
    return 0;
}