#include <bits/stdc++.h>
using namespace std;
const int N = 18;
long long a, b, base[N], f[N], g[N];
long long solve(long long n, int k) {
	if (n <= 0) return 0;
	vector<int> num;
	long long ans = 0, cnt = 0;
	while (n) {
		num.push_back(n % 10);
		n /= 10;
	}
	int m = num.size();
	for (int i = 1; i < m; ++i) {
		f[i] = f[i - 1] * 10 + base[i - 1];
		g[i] = g[i - 1] + 9 * f[i - 1] + base[i - 1] * (k != 0);
	}
	if (m > 1) ans += g[m - 1];
	for (int i = m - 1; ~i; i--) {
		for (int j = i == m - 1; j < num[i]; j++) {
			if (j == k) ans += base[i];
			ans += base[i] * cnt;
			ans += f[i];
		}
		if (num[i] == k) cnt++;
	}
	return ans + cnt;
}
int main() {
	base[0] = 1;
	for (int i = 1; i < N; i++) base[i] = base[i - 1] * 10;
	cin >> a >> b;
	for (int x = 0; x <= 9; x++) {
		cout << solve(b, x) - solve(a - 1, x) << " ";
	}
	return 0;
}
