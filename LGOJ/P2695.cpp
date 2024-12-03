#include <bits/stdc++.h>
using namespace std;
int n, m, a[20010], b[20010], ans;
int main() {
	cin >> n >> m;
	for (int i = 1; i <= n; i++) cin >> a[i];
	for (int j = 1; j <= m; j++) cin >> b[j];
	sort(a + 1, a + n + 1), sort(b + 1, b + m + 1);
	for (int i = 1, j = 1; i <= n; i++, j++) {
		while (j <= m && b[j] < a[i]) j++;
		if (j > m) {
			cout << "you died!";
			return 0;
		}
		ans += b[j];
	}
	cout << ans;
	return 0;
}
