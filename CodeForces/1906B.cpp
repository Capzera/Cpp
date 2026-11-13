#include <bits/stdc++.h>
using namespace std;
int main() {
    int t, n;
    cin >> t;
    while (t--) {
        cin >> n;
        vector<long long> nums(n);
        for (auto& x : nums) cin >> x;
        long long j = 2;
        while (1) {
            unordered_set<long long> s;
            for (auto& x : nums) {
                s.emplace(x % j);
            }
            if (s.size() == 2) break;
            j <<= 1;
        }
        cout << j << endl;
    }
    system("pause");
    return 0;
}