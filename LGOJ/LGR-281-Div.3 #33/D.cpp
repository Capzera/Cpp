#include <bits/stdc++.h>
using namespace std;
long long qpow(long long a, int b, long long p) {
	long long ans = 1;
	while (b) {
		if (b & 1) ans = ans * a % p;
		a = a * a % p;
		b >>= 1;
	}
	return ans;
}
long long k, p, mod, val, sum;
string s;
int main() {
	cin >> k >> p >> s;
	mod = 1ll << p;
	for (auto& c : s) {
		val = (val * 26 + (c - 'a')) % mod;
	}
	long long sz = s.size();
	for (auto& c : s) {
		sum = sum * 26 + (c - 'a');
		if (sum >= p) {
			sum = p;
			break;
		}
	}
	for (int i = 0; i < min(k, 3ll); i++) {
		if (sz >= p || !sum || sum == 1 && val == 1) break;
		long long pow = qpow(26, sz, mod), nxt = 0, base = 1;
		for (int j = 0; j < sum; j++) {
			nxt = (nxt + base * val) % mod;
			base = (base * pow) % mod;
			if (!base) break; // 'b'
		}
		val = nxt;
		sz = min(p, sz * sum);
		if (sum >= 2) sum = p;
	}
	cout << val;
	return 0;
}
/*
αβ = α*26^1 + β*26^0 = val
abababababa....2val
a
a * 26^107 + b * 26^106 + a... + b* 2^0
cc = (a*26^1 + b*26^0)26^106 = val;
val * (26^106 + 26^104 + ... + 26^0)
val * (2^L * 13^L * (26^105 + 26^103 + ... + 26^-1))
2^L * 13^L * (2^105 + 2^103 + ... + 2^-1) % 2^p
L >= p  nxt === 0 （mod 2^p）
a = 0
b = 1 = 1 = 1
c = cc = 54 > 20
d = ddd = 2109 > 20
a 0 0 0
b 1 1 1
c 2  cc 54  c 1966
d 3 ddd 2109
*/
