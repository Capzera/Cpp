#include <bits/stdc++.h>
using namespace std;
const int N = 1e4 + 10, M = 1e3 + 10;
int n, m, h1, m1, h2, m2, a, b, p, dp[M];
char ch;
int main() {
	cin >> h1 >> ch >> m1 >> h2 >> ch >> m2 >> n;
	m = h2 * 60 + m2 - h1 * 60 - m1;
	vector<int> w, c;
	for (int i = 1; i <= n; i++) {
		cin >> a >> b >> p;
		if (!p) p = 1000;
		for (int j = 1; j <= p; p -= j, j <<= 1) {
			w.push_back(j * a), c.push_back(j * b);
		}
		if (p) w.push_back(p * a), c.push_back(p * b);
	}
	for (int i = 0; i < w.size(); i++) {
		for (int j = m; j >= w[i]; j--) {
			dp[j] = max(dp[j], dp[j - w[i]] + c[i]);
		}
	}
	cout << dp[m];
	return 0;
}
