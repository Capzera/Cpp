#include <bits/stdc++.h>
using namespace std;
const int N = 5e5 + 10;
long long n, k, a[N], ans;
long long cat(long long a, long long b) {
	return stoll(to_string(a) + to_string(b));
}
int main() {
	cin >> n >> k;
	for (int i = 1; i <= n; i++) cin >> a[i];
	sort(a + 1, a + n + 1);
	for (int i = 1, j = n; i <= n; i++) {
		while (j && cat(a[i], a[j]) > k) j--;
		if (j >= i) ans--;
		ans += j;
	}
	cout << ans;
	return 0;
}
