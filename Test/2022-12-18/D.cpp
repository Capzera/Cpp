#include <bits/stdc++.h>
using namespace std;
int main() {  
    int n, m, j = 1;
    long long ans = 1;
    cin >> n >> m;
    if (n < m) swap(n, m);
    if (m < n - m) m = n - m;
	for (int i = m + 1; i <= n; i++) {
        ans *= i;
        if (ans % j == 0) ans /= j++;
    }
    for (; j <= n - m; j++) ans /= j;
	cout << ans << endl;
    system("pause");
    return 0;
}