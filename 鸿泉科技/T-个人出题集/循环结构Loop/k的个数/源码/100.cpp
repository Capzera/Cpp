#include <bits/stdc++.h>
using namespace std;
long long n, ans;
int main() {
	cin >> n;
	vector<long long> a(n + 1);
	for (int i = 1; i <= n; i++) cin >> a[i];
	for (int i = 2; i <= n; i++) {
		long long now = a[i];
		for (int j = 1; j <= sqrt(i); j++) {
			if (i % j) continue;
			a[i] = max(a[i], a[j] + now);
			if (j > 1) a[i] = max(a[i], a[i / j] + now);
		}
		ans = max(ans, a[i]);
	}
	cout << ans << endl;
	return 0;
}
