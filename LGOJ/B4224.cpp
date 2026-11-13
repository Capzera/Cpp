#include <bits/stdc++.h>
using namespace std;
int n, st[1000001];
vector<int> prime;
int main() {
	cin >> n;
	for (int i = 2; i <= n; i++) {
		if (!st[i]) prime.push_back(i);
		for (int j = 0; j < prime.size() && i * prime[j] <= n; j++) {
			st[i * prime[j]] = 1;
			if (i % prime[j] == 0) break;
		}
	}
	for (auto& p : prime) {
		long long cnt = 0;
		for (long long x = p; x <= n; x *= p) {
			cnt += n / x;
		}
		cout << p << " " << cnt << '\n';
	}
	return 0;
}

