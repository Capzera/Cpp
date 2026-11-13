#include <bits/stdc++.h>
using namespace std;
int n, st[10001];
int main() {
	vector<int> prime;
	cin >> n;
	for (int i = 2; i <= n; i++) {
		if (!st[i]) prime.emplace_back(i);
		for (int j = 0; j < prime.size() && i * prime[j] <= n; j++) {
			st[i * prime[j]] = 1;
			if (i % prime[j] == 0) break;
		}
	}
	set<int> p(prime.begin(), prime.end());
	for (int i = 4; i <= n; i += 2) {
		cout << i << '=';
		for (auto& x : p) {
			if (p.count(i - x)) {
				cout << x << '+' << i - x << endl;
				break;
			}
		}
	}
	return 0;
}
