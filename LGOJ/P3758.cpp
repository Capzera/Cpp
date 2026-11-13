#include <bits/stdc++.h>
using namespace std;
const int mod = 2017;
long long n, m, u, v, t, ans;
struct Matrix {
	int n = 30;
	long long val[31][31] = {0};
	void build() {
		for (int i = 1; i <= n; i++) val[i][i] = 1;
	}
	Matrix operator * (const struct Matrix& rhs) const {
		Matrix ans;
		for (int i = 1; i <= n; i++) {
			for (int j = 1; j <= n; j++) {
				for (int k = 1; k <= n; k++) {
					ans.val[i][j] = (ans.val[i][j] + val[i][k] * rhs.val[k][j]) % mod;
				}
			}
		}
		return ans;
	}
};
Matrix qpow(Matrix a, long long b) {
	Matrix ans;
	ans.build();
	while (b) {
		if (b & 1) ans = ans * a;
		b >>= 1;
		a = a * a;
	}
	return ans;
}
int main() {
	cin >> n >> m;
	Matrix base;
	while (m--) {
		cin >> u >> v;
		base.val[u][v] = base.val[v][u] = 1;
	}
	for (int i = 0; i <= n; i++) base.val[i][i] = base.val[i][0] = 1;
	base.val[0][0] = 0;
	cin >> t;
	base = qpow(base, t);
	for (int i = 0; i <= n; i++) {
		ans = (ans + base.val[1][i]) % mod;
	}
	cout << ans;
	return 0;
}
