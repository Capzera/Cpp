#include <bits/stdc++.h>
using namespace std;
const int N = 1e6 + 10, p1 = 13331, m1 = 1e9 + 7;
int t, n;
string s;
long long h[N], p[N];
long long get(int l, int r) {
	return (h[r] - h[l - 1] * p[r - l + 1] % m1 + m1) % m1;
}
int solve() {
	cin >> s;
	n = s.size();
	long long ans = 0, j = 0;
	for (int i = 1; i <= n; i++) {
		h[i] = (h[i - 1] * p1 + s[i - 1]) % m1;
	}
	for (int i = 1; i <= n / 2; i++) {
		if (get(j + 1, i) == get(n - i + 1, n - j)) {
			ans += 2;
			j = i;
		}
	}
	if (j < n / 2 || n % 2) ans++;
	return ans;
}

int main() {
	p[0] = 1;
	for (int i = 1; i <= 1e6; i++) p[i] = p[i - 1] * p1 % m1;
	cin >> t;
	while (t--) cout << solve() << '\n';
	return 0;
}
