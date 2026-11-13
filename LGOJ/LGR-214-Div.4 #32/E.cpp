#include <bits/stdc++.h>
using namespace std;
int n, k, ans, a[5001];
int main() {
	cin >> n >> k;
	for (int i = 1; i <= n; i++) cin >> a[i];
	for (int i = 1; i <= n - k + 1; i++) {
		for (int j = i; j <= i + k - 1; j++) {
			a[j] -= 2;
		}
		int win = 0;
		for (int j = 1; j <= n; j++) {
			win += a[j] < 50;
		}
		ans += win * 2 > n;
		for (int j = i; j <= i + k - 1; j++) {
			a[j] += 2;
		}
	}
	cout << ans;
	return 0;
}
