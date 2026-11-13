#include <bits/stdc++.h>
using namespace std;
int n, ans, a[100001];
int main() {
	cin >> n;
	for (int i = 1; i <= n; i++) cin >> a[i];
	sort(a + 1, a + n + 1);
	n = unique(a + 1, a + n + 1) - a;
	for (int i = 1, j = 1; i <= n; i++) {
		if (a[i] - a[j] != i - j) j = i;
		ans = max(ans, i - j + 1);
	}
	cout << ans;
	return 0;
}
