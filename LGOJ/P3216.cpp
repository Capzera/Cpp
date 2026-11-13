#include <bits/stdc++.h>
using namespace std;
unsigned long long n;
int m;
struct Matrix {
	long long n = 0, m = 0, p = 0;
	unsigned long long val[3][3] = {0};
	Matrix() {}
	Matrix(int _n, int _m, int _p) {
		n = _n, m = _m, p = _p;
	}
	Matrix operator * (const struct Matrix& rhs) {
		Matrix ans(n, rhs.m, p);
		for (int r = 0; r < n; r++) {
			for (int c = 0; c < rhs.m; c++) {
				for (int i = 0; i < m; i++) {
					ans.val[r][c] = (ans.val[r][c] + val[r][i] * rhs.val[i][c]) % p;
				}
			}
		}
		return ans;
	}
	~Matrix() {}
};
Matrix qpow(Matrix ans, Matrix a, long long b) {
	while (b) {
		if (b & 1) ans = ans * a;
		a = a * a;
		b >>= 1;
	}
	return ans;
}
int main() {
	cin >> n >> m;
	Matrix base(3, 3, m), ans(1, 3, m);
	base.val[1][0] = base.val[1][1] = base.val[2][1] = base.val[2][2] = 1;
	ans.val[0][1] = ans.val[0][2] = 1;
	for (unsigned long long k = 10; ; k *= 10) { // 18
		base.val[0][0] = k % m;
		if (k > n) {
			ans = qpow(ans, base, n - k / 10 + 1); // 1 + 2 + 3 + .. + 18
			break;
		}
		ans = qpow(ans, base, k - k / 10);
	}
	cout << ans.val[0][0] << '\n';
	return 0;
}

