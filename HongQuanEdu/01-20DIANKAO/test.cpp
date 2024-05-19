#include <bits/stdc++.h>
using namespace std;
int a[1005], b[1005], n, m, k;
long long sum;
int main() {
    cin >> n >> m >> k;
    for (int i = 0; i < n; i++) cin >> a[i];
    for (int j = 0; j < m; j++) cin >> b[j];
    sort(a, a + n);
    sort(b, b + m);
    for (int i = 1; i <= k; i++) {
        if (i % 2 == 1) {
            if (a[0] < b[m - 1]) {
                swap(a[0], b[m - 1]);
            }
        }
        if (i % 2  == 0) {
            if (b[0] < a[n - 1]) {
                swap(b[0], a[n - 1]);
            }
        }
        sort(a, a + n);
        sort(b, b + m);
    }
    for (int i = 0 ; i < n; i++) {
        sum += a[i];
    }
    cout << sum << endl;
    system("pause");
    return 0;
}