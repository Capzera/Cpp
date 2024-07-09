#include <bits/stdc++.h>
using namespace std;
int st[5000001];
int main() {
	//freopen("prime.in", "r", stdin);
	//freopen("prime.out, “w", stdout);
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
		int loc = INT_MAX, ans = 0;
		for (int j = 0; prime[j] < x + 100; j++) {
			//if (prime[j] - x > loc && prime[j] > x) break;
			if (abs(prime[j] - x) < loc) {
				loc = abs(prime[j] - x);
				ans = prime[j];
			}
		}
		cout << ans << " ";
	}
	return 0;
}
