#include <bits/stdc++.h>
using namespace std;
long long n, s, m, x, ans, a[1001], sum[1001];
int main() {
	cin >> n >> s >> m; s--;
	a[s]++;
	while (m--) {
		cin >> x;
		if (x == 1) s = (s + 1 + n) % n;
		else s = (s - 1 + n) % n;
		a[s]++;
	}
	for (int i = 0; i < n; i++) {
		cin >> m;
		for (int j = 1; j <= m; j++) cin >> sum[j], sum[j] += sum[j - 1];
		ans += sum[min(a[i], m)];
	}
	cout << ans;
	return 0;
}
