#include <bits/stdc++.h>
using namespace std;
const int N = 2010;
int n, ans, a[N], b[N];
int main() {
	cin >> n;
	for (int i = 1; i <= n; i++) cin >> a[i];
	for (int i = 1; i <= n; i++) cin >> b[i];
	sort(a + 1, a + n + 1, greater<long long>());
	sort(b + 1, b + n + 1, greater<long long>());
	for (int i = 1, j = 1; i <= n && j <= n; i++, j++) {
		while (j <= n && a[i] <= b[j]) {
			ans--, j++;
		}
		if (j > n) break;
		if (a[i] > b[j]) ans++;
	}
	cout << ans * 2;
	return 0;
}
