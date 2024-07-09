#include <bits/stdc++.h>
using namespace std;
int st[5000001];
int main() {
	freopen("prime.in", "r", stdin);
	freopen("prime.out", "w", stdout);
	memset(st, 0, sizeof(st));
	int n, x;
	cin >> n;
	vector<int> prime;
	for (int i = 2; i <= 5e6; i++) {
		if (!st[i]) prime.push_back(i);
		for (int j = 0; j < prime.size() && i * prime[j] <= 5e6; j++) {
			st[i * prime[j]] = 1;
			if (i % prime[j] == 0) break;
		}
	}
	for (int i = 0; i < n; i++) {
		cin >> x;
		int y = upper_bound(prime.begin(), prime.end(), x) - prime.begin();
		if (!y) {
			cout << 2 << " ";
			continue;
		}
		if (abs(prime[y] - x) < abs(prime[y - 1] - x)) {
			cout << prime[y] << " ";
		} else cout << prime[y - 1] << " ";
	}
	return 0;
}
