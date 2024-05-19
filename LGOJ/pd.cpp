#include <bits/stdc++.h>
using namespace std;
int a[10010], ans, n, m;
int main() {
	cin >> n >> m;
	for (int i = 1; i <= n; i++) cin >> a[i];
	for (int i = 1; i <= n - m; i++) {
		sort(a + 1, a + m + 1);
		a[1] += a[m + i];
	}
	sort(a + 1, a + m + 1);
	cout << a[m];
	return 0;
}
