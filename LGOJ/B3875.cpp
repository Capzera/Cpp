#include <bits/stdc++.h>
using namespace std;
long long x, n, ans, mx, cnt[1001];
int main() {
	cin >> n >> x;
	for (int i = 0; i < n; i++) {
		cnt[x]++;
		x = (37 * x + 33031) % n + 1;
	}
	for (int i = 1; i <= n; i++) {
		if (cnt[i] > mx) {
			mx = cnt[i];
			ans = i;
		}
	}
	cout << ans;
	return 0;
}
