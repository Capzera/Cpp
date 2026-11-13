#include <bits/stdc++.h>
using namespace std;
int n, a[1001], ans;
int main() {
	cin >> n;
	for (int i = 1; i <= n; i++) cin >> a[i];
	for (int i = 1; i < n; i++) {
		for (int j = i + 1; j <= n; j++) {
			int sum = a[i] + a[j];
			int sq = sqrt(sum);
			if (sq * sq == sum) ans++;
		}
	}
	cout << ans;
	return 0;
}
