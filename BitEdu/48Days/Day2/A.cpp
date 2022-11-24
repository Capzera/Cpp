#include <bits/stdc++.h>
using namespace std;
int main() {
    int n, ans = 1, flag = -1;
    cin >> n;
    vector<int> nums(n);
    for (int i = 0; i < n; i++) cin >> nums[i];
    for (int i = 1; i < n; i++) {
        if (nums[i] == nums[i - 1]) continue;
        if (flag == -1) flag = nums[i] > nums[i - 1];
        if ((nums[i] > nums[i - 1]) ^ flag) {
            ans++;
            flag = -1;
        }
    }
    cout << ans << endl;
    return 0;
}
/*
l + (r - l) << 1;
*/