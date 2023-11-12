#include <bits/stdc++.h>
using namespace std;
int solve() {
    int x;
    int cnt1 = 0, cnt2 = 0;
    for (int i = 0; i < 8; i++) {
        cin >> x;
        if (x >= 41) cnt1 ++;
        if (x >= 63) cnt2 ++;
        if (x >= 118) return 10;
    }
    if (cnt2 >= 4) return 8;
    if (cnt1 >= 4) return 3;
    return 1;
}
int main() {
    cout << solve() << endl;
    system("pause");
    return 0;
}