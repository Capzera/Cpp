#include <bits/stdc++.h>
using namespace std;
const int N = 3e5 + 5;
long long n, k, nums[N];
int check(long long x) {
    long long cnt = 0;
    for (int i = 0; i < n; i++) {
        cnt += (nums[i] + x - 1) / x;
    }
    return cnt <= k;
}
int main() {
    cin >> k >> n;
    long long l = 0, r = 0;
    for (int i = 0; i < n; i++) {
        cin >> nums[i];
        r += nums[i];
    }
    while (r - l > 1) {
        long long mid = l + (r - l) / 2;
        if (check(mid)) r = mid;
        else l = mid;
    }
    cout << r << endl;
    system("pause");
    return 0;
}