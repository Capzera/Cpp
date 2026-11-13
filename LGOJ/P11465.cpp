#include <bits/stdc++.h>
using namespace std;
const int p = 1e9 + 7;
long long qpow(long long a, int b) {
	long long ans = 1;
	while (b) {
		if (b & 1) ans = ans * a % p;
		a = a * a % p;
		b >>= 1;
	}
	return ans;
}
long long n, k, t;
int main() {
	cin >> t;
	while (t--) {
		cin >> n >> k;
		cout << 1ll * (k + 1) * (qpow(k + 1, n) - 1) % p * qpow(k, p - 2) % p << endl;
	}
	return 0;
}
/*
还剩x张消耗为1的设为Ax
还剩x张消耗为0的设为Bx
先用了一张An
An  -> k * Bn + An-1
k * Bn -> k * An-1
An -> (k + 1) An-1
An = (k + 1) * An-1
a1 = k + 1 q = k + 1
(k + 1) * (k + 1)^n - 1
-------------------------
k

*/
