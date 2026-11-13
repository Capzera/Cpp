#include <bits/stdc++.h>
using namespace std;
int n, cnt, ans = INT_MAX;
bool st[10010];
vector<int> prime;
bool check(int num) {
	for (int m = 2; m * m <= num; m++) {
		if (num % (m * m) == 0) return 1;
	}
	return 0;
}
int main() {
	cin >> n;
	for (int i = 2; i < n; i++) {
		if (!st[i]) prime.push_back(i);
		for (int j = 0; j < prime.size() && i * prime[j] < n; j++) {
			st[i * prime[j]] = 1;
			if (i % prime[j] == 0) break;
		}
	}
	for (auto& p : prime) {
		int num = n - p;
		if (!check(num)) continue;
		cnt++;
		ans = min(ans, abs(num - p));
	}
	cout << cnt << endl << ans;
	return 0;
}
