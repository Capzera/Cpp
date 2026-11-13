#include <bits/stdc++.h>
using namespace std;
long long p, q, a1, a2, n, mod;
struct Matrix {
	int n = 2;
	long long val[4][4] = {0};
	void build() {
		for (int i = 0; i < n; i++) val[i][i] = 1;
	}
	Matrix operator * (const struct Matrix& rhs) const {
		Matrix ans;
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				for (int k = 0; k < n; k++) {
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
	cin >> p >> q >> a1 >> a2 >> n >> mod;
	Matrix base;
	base.val[0][0] = p, base.val[1][0] = q, base.val[0][1] = 1;
	if (n == 1) return cout << a1, 0;
	if (n == 2) return cout << a2, 0;
	base = qpow(base, n - 2);
	cout << (a2 * base.val[0][0] % mod + a1 * base.val[1][0] % mod) % mod;
	return 0;
}
