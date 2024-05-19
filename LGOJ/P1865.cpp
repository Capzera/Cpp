#include <bits/stdc++.h>
using namespace std;
int n, m, l, r;
int st[1000001], sum[1000001];
int main() {
	cin >> n >> m;
	
	vector<int> prime;
	for (int i = 2; i <= m; i++) {
		if (!st[i]) prime.emplace_back(i);
		for (int j = 0; j < prime.size() && i * prime[j] <= m; j++) {
			st[i * prime[j]] = 1;
			if (i % prime[j] == 0) break;
		}
	}
	for (int i = 2; i <= m; i++) {
		sum[i] = sum[i - 1] + (st[i] == 0);
	}
	while (n--) {
		cin >> l >> r;
		if (r > m || l > m || r < 1 || l < 1) {
			cout << "Crossing the line" << endl;
			continue;
		}
		cout << sum[r] - sum[l - 1] << endl;
	}
	return 0;
}
