#include <bits/stdc++.h>
using namespace std;
int main() {
    int moon[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}, ans = 0;
    set<int> mp = {9, 16, 25};
    auto get = [](int n) -> int {return n % 10 + n / 10 % 10;};
    for (int y = 1; y < 22; y++) {
        moon[2] = 29 - (y % 4 != 0);
        for (int m = 1; m < 13; m++) {
            for (int d = 1; d <= moon[m]; d++) {
                int cnt = 2 + get(y) + get(m) + get(d);
                ans += mp.count(cnt);
            }
        }
    }
    cout << ans << endl;
    system("pause");
    return 0;
}