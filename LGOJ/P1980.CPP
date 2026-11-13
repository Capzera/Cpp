#include <bits/stdc++.h>
using namespace std;
int main() {
    int n, k, ans = 0;
    cin >> n >> k;
    for (int i = 1; i <= n; i++) {
        int num = i;
        while (num) {
            ans += num % 10 == k;
            num /= 10;
        }
    }
    cout << ans << endl;
    system("pause");
    return 0;
}