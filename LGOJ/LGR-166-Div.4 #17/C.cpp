#include <bits/stdc++.h>
using namespace std;
int solve() {
    int x, cnt1 = 0, cnt2 = 0;
    for (int i = 0; i < 8; i++) {
        cin >> x;
        if (x >= 118) return 10;
        cnt1 += x >= 41;
        cnt2 += x >= 63;
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