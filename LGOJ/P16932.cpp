#include <bits/stdc++.h>
using namespace std;
long long n, x, y, z, magic, cnt = 1, ans = LLONG_MAX;
int main() {
	cin >> n >> x >> y >> z;
	for (long long t = 1; ; t++) {
		cnt += magic / x;
		magic %= x;
		magic += cnt * y;
		long long need = max(0LL, n - magic - cnt * z); 
		ans = min(ans, t + (need + cnt * y - 1) / (cnt * y));
		if (x <= z) {
			magic += cnt * z;
			cnt = 0;
		}
		if (magic + cnt * z >= n) {
			cout << min(t, ans);
			return 0;
		}
	}
	return 0;
} 
