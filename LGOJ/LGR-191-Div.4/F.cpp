#include <bits/stdc++.h>
using namespace std;
long long n, cnt, sum, ans = LLONG_MAX;
long long a[1001];
int main() {
	cin >> n;
	for (int i = 1; i <= n; i++) {
		cin >> a[i];
		sum += a[i];
	}
	sort(a, a + n);
	for (long long i = 1; i <= n; i++) {
		cnt += a[i];
		long long bonus = min(i, sum - cnt);
		ans = min(ans, sum - bonus);
	}
	cout << ans;
	return 0;
}
