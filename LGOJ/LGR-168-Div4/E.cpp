#include <bits/stdc++.h>
using namespace std;
int main() {
    int m, n;
    cin >> m >> n;
    vector<int> nums(n);
    for (int i = 0; i < m; i++) {
        for (auto& x : nums) cin >> x;
        int loc = -1, mx = -1, cnt = 0;
        for (int j = n - 1; j >= 0; j--) {
            if (!nums[j]) continue;
            if (nums[j] > mx) {
                mx = nums[j];
                loc = j + 1;
                cnt = 0;
            }
            else cnt++;
        }
        cout << loc << " " << cnt << endl;
    }
    system("pause");
    return 0;
}