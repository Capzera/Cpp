#include <bits/stdc++.h>
using namespace std;
const int mod = 1e9 + 7;
struct Matrix {
	int n = 3;
	long long val[4][4] = {0};
	void build() {
		for (int i = 0; i < 3; i++) val[i][i] = 1;
	}
	Matrix operator * (const struct Matrix& rhs) const {
		Matrix ans;
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				for (int k = 0; k < 3; k++) {
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

long long T, n; 
long long solve() {
	scanf("%lld", &n);
	if (n < 4) return 1ll;
	Matrix base;
	base.val[0][0] = base.val[0][1] = base.val[1][2] = base.val[2][0] = 1;
	base = qpow(base, n - 3);
	return (base.val[0][0] + base.val[1][0] + base.val[2][0]) % mod;
}
int main() {
	
	scanf("%lld", &T);
	while (T--) {
		printf("%lld\n", solve());
	}
	return 0;
}
