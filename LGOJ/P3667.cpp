#include <bits/stdc++.h>
using namespace std;
const int N = 501, p1 = 131, m1 = 1e9 + 7;
int n, m;
string s;
long long h[N << 1][N], p[N], hs[N];
long long get(int i, int l, int r) {
	return (h[i][r] - h[i][l - 1] * p[r - l + 1] % m1 + m1) % m1;
}
int check(int len) {
	for (int l = 1; l + len - 1 <= m; l++) {
		int r = l + len - 1, f = 1;
		for (int i = 1; i <= n; i++) { // 斑点牛
			hs[i] = get(i, l, r);
		}
		sort(hs + 1, hs + n + 1);
		for (int i = n + 1; i <= n * 2; i++) {
			long long cur = get(i, l, r);
			if (binary_search(hs + 1, hs + n + 1, cur)) {
				f = 0;
				break;	
			}
		}
		if (f) return 1;
	}
	return 0;
}
int main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	cin >> n >> m;
	p[0] = 1;
	for (int i = 1; i <= m; i++) p[i] = p[i - 1] * p1 % m1;
	for (int i = 1; i <= 2 * n; i++) {
		cin >> s;
		for (int j = 1; j <= m; j++) {
			h[i][j] = (h[i][j - 1] * p1 + s[j - 1]) % m1;
		}
	}
	int l = 1, r = m + 1, ans = m;
	while (l < r) {
		int mid = l + (r - l >> 1);
		if (check(mid)) {
			r = ans = mid;
		} else l = mid + 1;
	}
	cout << ans;
	return 0;
}
