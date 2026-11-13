#include <bits/stdc++.h>
using namespace std;
int main() {
    vector<int> nums;
    int temp;
    while (cin >> temp) {
        nums.push_back(temp);
        if (cin.get() == '\n') break;
    }
    int n = nums.size(), ans = 0;
    vector<int> left(n, 1), right(n, 1);
    for (int i = 1; i < n; i++) {
        if (nums[i] > nums[i - 1]) 
            left[i] = left[i - 1] + 1;
    }
    for (int i = n - 2; i >= 0; i--) {
        if (nums[i] > nums[i + 1]) 
            right[i] = right[i + 1] + 1;
    }
    for (int i = 0; i < n; i++) ans += max(left[i], right[i]);
    cout << ans << endl;
    system("pause");
    return 0;
}
/*
3 2 1 2 3 3 3 1
3 2 1 2 3 1 2 1
*/