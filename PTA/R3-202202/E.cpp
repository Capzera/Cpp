#include <bits/stdc++.h>
using namespace std;
const int N = 1e3 + 10, p = 1e9 + 7;
string s;
int k, n, ans, C[N][N], step[N];
int solve(int c) {
	int pre = 0, ans = 0;
	for (int i = 0; i < n; i++) {
		if (s[i] == '1') {
			int need = c - pre;
			if (pre <= c && need <= n - i - 1) {
				ans = (ans + C[n - i - 1][need]) % p;
			}
			pre++;
		}
	}
	if (pre == c) ans = (ans + 1) % p;
	return ans;
}
int main() {
	cin >> s >> k;
	if (!k) return puts("1"), 0;
	int n = s.size();
	for (int i = 0; i <= n; i++) {
		C[i][0] = C[i][i] = 1;
		for (int j = 1; j < i; j++) {
			C[i][j] = (C[i - 1][j - 1] + C[i - 1][j]) % p;
		}
	}
	for (int i = 2; i < N; ++i) {
		int pc = __builtin_popcount(i);
		step[i] = step[pc] + 1;
	}
	for (int c = 1; c <= n; ++c) {
		if (c <= N && step[c] == k - 1) {
			int cnt = solve(c);
			if (c == 1) cnt = (cnt - 1 + p) % p;
			ans = (ans + cnt) % p;
		}
	}
	cout << ans;
	return 0;
}
