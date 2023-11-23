#include <bits/stdc++.h>
using namespace std;
long long cards[100010], sum[100011];
int main() {
    cin.tie(nullptr)->sync_with_stdio(0);
    int n, x;
    long long sum = 0, ans = 0;
    cin >> n;
    for (int i = 0; i < n; i++) {
        cin >> x;
        sum += x;
        if (i && sum > 0) ans += sum;
    }
    cout << ans << endl;
    return 0;
}