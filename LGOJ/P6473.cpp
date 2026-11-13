#include <bits/stdc++.h>
using namespace std;
const int N = 2e5 + 10;
long long n, L, v, q, x;
long long a[N], s[N];
int main() {
	cin >> n >> L >> v;
	for (int i = 1; i <= n; i++) cin >> a[i];
	sort(a + 1, a + n + 1, greater<int>());
	for (int i = 1; i <= n; i++) s[i] = s[i - 1] + a[i];
	cin >> q;
	while (q--) {
		cin >> x;
		x = x * v - L;
		if (x < 0) puts("0");
		else if (s[n] <= x)  puts("-1");
		else cout << upper_bound(s + 1, s + n + 1, x) - s << '\n';
	}
	return 0;
}

