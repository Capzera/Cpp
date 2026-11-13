#include <bits/stdc++.h>
using namespace std;
const int p = 10007;
long long n, a[10001], phi[p + 10];
bool st[p + 10];
vector<int> prime;
long long read() {
	long long ans = 0;
	char ch = getchar();
	while (!isdigit(ch)) ch = getchar();
	while (isdigit(ch)) {
		ans = ans * 10 + ch - '0';
		ch = getchar();
	}
	return ans;
}

long long qpow(long long a, int b, int p) {
	long long ans = 1;
	while (b) {
		if (b & 1) {
			ans = ans * a;
			if (ans >= p) ans = ans % p + p;
		}
		b >>= 1;
		a = a * a;
		if (a >= p) {
			a = a % p + p;
		}
	}
	return ans;
}

long long solve(int i, int mod) {
	if (i > n || mod == 1) return 1;
	return qpow(a[i], solve(i + 1, phi[mod]), mod);
}

void sieve(int n) {
	phi[1] = 1;
	for (int i = 2; i <= n; i++) {
		if (!st[i]) {
			prime.push_back(i);
			phi[i] = i - 1;
		}
		for (int j = 0; j < prime.size() && i * prime[j] <= n; j++) {
			st[i * prime[j]] = 1;
			if (i % prime[j] == 0) {
				phi[i * prime[j]] = phi[i] * prime[j];
				break;
			} else {
				phi[i * prime[j]] = phi[i] * phi[prime[j]];
			}
		}
	}
}

int main() {
	n = read();
	sieve(10009);
	for (int i = 1; i <= n; i++) a[i] = read();
	cout << solve(1, p) % p;
	return 0;
}
