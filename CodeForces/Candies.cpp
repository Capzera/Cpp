#include <bits/stdc++.h>
using namespace std;
int n, k, l, r, ans;
int a[200010];
int main() {
    cin >> n >> k;
    for (int i = 0; i < n; i++) {
        cin >> a[i];
    }
    sort(a, a + n);
    long long sum = 0;
    for (int i = n / 2; i < n; i++) {
        sum += a[i];
    }
    cout << (sum + k) / (n / 2 + 1) << endl;
    system("pause");
    return 0;
}