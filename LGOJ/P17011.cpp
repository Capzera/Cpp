#include <bits/stdc++.h>
using namespace std;
long long n, ans, a[1001];
int main() {
	cin >> n;
	for (int i = 1; i <= n; i++) {
		cin >> a[i];
		for (int j = 1; j < i; j++) {
			if (__gcd(a[i], a[j]) > 1) continue;
			ans = max(ans, a[i] + a[j]);
		}
	}
	cout << ans;
	return 0;
}
