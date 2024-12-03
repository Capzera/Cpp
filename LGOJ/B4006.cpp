#include <bits/stdc++.h>
using namespace std;
long long n, k, a[1001], ans;
int main() {
	cin >> n >> k;
	for (int i = 1; i <= n; i++) cin >> a[i];
	sort(a + 1, a + n + 1);
	for (int i = 1; i <= n; i++) {
		long long sum = a[i];
		for (int j = i + 1; j <= n; j++) {
			if (a[j] - a[i] > k) break;
			sum += a[j];
		}
		ans = max(ans, sum);
	}
	cout << ans;
	return 0;
}
