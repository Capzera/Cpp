#include <bits/stdc++.h>
using namespace std;
int n, s[10], b[10], ans = INT_MAX;
int main() {
	cin >> n;
	for (int i = 0; i < n; i++) cin >> s[i] >> b[i];
	for (int i = 1; i < 1 << n; i++) {
		int sum = 0, pro = 1;
		for (int j = 0; j < n; j++) {
			if (i >> j & 1 ^ 1) continue;
			sum += b[j];
			pro *= s[j];
		}
		ans = min(ans, abs(sum - pro));
	}
	cout << ans;
	return 0;
}
