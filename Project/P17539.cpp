#include <bits/stdc++.h>
using namespace std;
const int N = 2e6 + 5;
int n, m, l, r, ans, sum[N];
string s;
int main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	cin >> n >> m >> s;
	for (int i = 1; i <= n; i++) {
		sum[i] = sum[i - 1] + (s[i - 1] == '0' ? 1 : -1);
	}
	while (m--) {
		cin >> l >> r;
		if ((r - l + 1) % 2) return puts("-1"), 0;
		ans += sum[r] != sum[l - 1];
	}
	cout << (ans + 1) / 2;
	return 0;
}
