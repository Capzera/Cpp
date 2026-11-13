#include <bits/stdc++.h>
using namespace std;
const int p = 2023;
long long n, phi[p + 10];
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

long long solve(int i, int p) {
	if (i == 2023 || p == 1) return 1;
	return qpow(i + 1, solve(i + 1, phi[p]), p);
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
	sieve(p + 2);
	cout << solve(1, p) % p;
	return 0;
}
