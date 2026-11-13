#include <bits/stdc++.h>
using namespace std;
int main() {
    int x;
    vector<int> nums;
    while (cin >> x, x) {
        nums.emplace_back(x);
    }
    reverse(nums.begin(), nums.end());
    for (auto& x : nums) cout << x << " ";
    system("pause");
    return 0;
}