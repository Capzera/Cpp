#include <bits/stdc++.h>
#define ll long long
using namespace std;
const int N = 1e7;
ll p;
bool st[N + 5];
int phi[N + 5]; 
vector<int> prime;

void sieve() {
	phi[1] = 1;
	for (int i = 2; i <= N; i++) {
		if (!st[i]) {
			prime.push_back(i);
			phi[i] = i - 1;
		}
		for (int j = 0; j < prime.size() && i * prime[j] <= N; j++) {
			st[i * prime[j]] = 1;
			if (i % prime[j] == 0) {
				phi[i * prime[j]] = phi[i] * prime[j];
				break;
			} else {
				phi[i * prime[j]] = phi[i] * (prime[j] - 1);
			}
		}
	}
}

ll qpow(ll a, ll b, ll p) {
	ll ans = 1;
	while (b) {
		if (b & 1) ans = ans * a % p;
		b >>= 1;
		a = a * a % p;
	}
	return ans;
}

ll fun(int p) {
	if (p == 1) return 0;
	return qpow(2, fun(phi[p]) + phi[p], p);
}

int main() {
	sieve();
	int t = 1;
	cin >> t;
	while (t--) {
		cin >> p;
		cout << fun(p) << '\n';
		
	}
	return 0;
}

