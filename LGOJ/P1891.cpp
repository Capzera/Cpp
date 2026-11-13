#include <bits/stdc++.h>
using namespace std;
const int N = 1e6;
int t, n;
long long phi[N + 5];
bool st[N + 5];
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
			} else phi[i * prime[j]] = phi[i] * (prime[j] - 1);
		}
	}
}
int main() {
	sieve();
	cin >> t;
	while (t--) {
		scanf("%d", &n);
		long long ans = 0, m = sqrt(n);
		for (int d = 1; d <= m; d++) {
			if (n % d) continue;
			ans += d * phi[d];
			if (d * d == n) break;
			ans += n / d * phi[n / d];
		}
		cout << (ans + 1) / 2 * n << '\n';
	}
	return 0;
}
