#include <bits/stdc++.h>
using namespace std;
const int N = 2e6 + 10;
bool st[N];
vector<int> prime;
long long n, ans, phi[N];

void sieve(int n) {
	prime.clear();
	memset(phi, 0, sizeof(phi));
	phi[1] = st[1] = 1;
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
	for (int i = 2; i <= n; i++) phi[i] += phi[i - 1];
}

int main() {
	cin >> n;
	sieve(n);
	for (int i = 1; i <= n; i++) {
		ans += i * (phi[n / i] - 1);
	}
	cout << ans;
	return 0;
}
