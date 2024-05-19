#include<bits/stdc++.h>
using namespace std;
int main(){
    int n, k, sum;
    cin >> n >> k;
    vector<int> nums(n);
    for (auto& x : nums) cin >> x;
    auto check = [&](int x) -> bool {
        int cnt = 0, sum = 0;
        for (auto& num : nums) {
            if (num > x) return 1;
            if (sum + num <= x) sum += num;
            else {
                sum = num, cnt++;
            }
        }
        return cnt >= k;
    };
    int l = 0, r = 1e9 + 1;
    while (l < r) {
        int x = l + (r - l) / 2;
        if (check(x)) l = x + 1;
        else r = x;
    }
    cout << r << endl;
    system("pause");
    return 0;
}