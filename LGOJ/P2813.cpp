#include <bits/stdc++.h>
using namespace std;
int m, n, ans, j = 1, atk[100010], def[100010];
int main() {
	cin >> m >> n;
	for (int i = 1; i <= m; i++) cin >> def[i];
	for (int i = 1; i <= n; i++) cin >> atk[i];
	sort(atk + 1, atk + n + 1);
	sort(def + 1, def + m + 1);
	for (int i = 1; i <= m; i++, j++) {
		while (j <= n && atk[j] <= def[i]) {
			ans += atk[j++];
		}
		if (j > n) return puts("0"), 0;
	}
	while (j <= n) ans += atk[j++];
	cout << ans;
	return 0;
}
