#include <bits/stdc++.h>
using namespace std;
int main() {
    unordered_set<long long> s;
    vector<long long> base(10); base[0] = 1;
    for (int i = 1; i < 10; i++) base[i] = base[i - 1] * 10;
    long long L, R, ans = 0, num;
    cin >> L >> R;
    for (int i = 0; i < 6; i++) {
        if (2022 * base[i] > R) break;
        for (int j = 0; j < base[i]; j++) {
            num = 2022 * base[i] + j;
            for (int n = 0; num <= R && n < base[5 - i]; n++, num += base[i + 4]) {
                if(num < L || s.count(num)) continue;
                s.insert(num);
                ans += num;
            }
        }
    }
    cout << ans << endl;
    system("pause");
    return 0;
}