#include <bits/stdc++.h>
using namespace std;
int main() {
    int n, f1 = 1, f0 = 0, ans = 0;
    cin >> n;
    while (!ans || f1 != 1 || f0) {
        int f2 = f0;
        f0 = (f0 + f1) % n;
        f1 = f2;
        ans++;
    }
    cout << ans << endl;
    system("pause");
    return 0;
}