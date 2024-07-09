#include <bits/stdc++.h>
using namespace std;
int main() {
    long long n;
    cin >> n;
    long long ans = n, meng = 0;
    while (n > 4) {
        meng = n / 5 * 2;
        ans += meng;
        n = n % 5 + meng;
    }
    cout << ans << endl;
    return 0;
}
