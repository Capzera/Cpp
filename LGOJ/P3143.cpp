#include <bits/stdc++.h>
using namespace std;
const int N = 5e4 + 10;
int n, k, ans, a[N], pre[N], suf[N];
int main() {
	cin >> n >> k;
	for (int i = 1; i <= n; i++) cin >> a[i];
	sort(a + 1, a + n + 1);
	for (int i = 1, j = 1; i <= n; i++) {
		while (j < i && a[i] - a[j] > k) j++;
		pre[i] = max(pre[i - 1], i - j + 1);
	}
	for (int i = n, j = n; i > 0; i--) {
		while (j > i && a[j] - a[i] > k) j--;
		suf[i] = max(suf[i + 1], j - i + 1);
	}
	for (int i = 1; i <= n; i++) {
		ans = max(ans, pre[i] + suf[i + 1]);
	}
	cout << ans;
	return 0;
}
