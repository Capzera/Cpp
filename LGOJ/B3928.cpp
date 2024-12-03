#include <bits/stdc++.h>
using namespace std;
long long n, a[50010], b[50010], ans;
int main() {
	cin >> n;
	for (int i = 1; i <= n; i++) cin >> a[i];
	for (int i = 1; i <= n; i++) cin >> b[i];
	sort(a + 1, a + n + 1), sort(b + 1, b + n + 1);
	for (int i = 1, j = 1; j <= n; i++, j++) {
		while (i <= n && a[i] < b[j]) i++;
		if (i > n) break;
		ans++;
	}
	cout << ans;
	return 0;
}
/*
3 5 7 8 10
1 2 4 6 9
*/
