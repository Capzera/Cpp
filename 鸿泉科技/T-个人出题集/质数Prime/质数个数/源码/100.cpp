#include <bits/stdc++.h>
using namespace std;
int st[5000010];
int main() {
	memset(st, 0, sizeof(st));
	int x, l, r, ans = 0;
	vector<int> prime;
	cin >> x >> l >> r;
	if (l > r) swap(l, r);
	for (int i = 2; i <= x; i++) {
		for (int j = 0; j < prime.size() && i * prime[j] <= x; j++) {
			st[i * prime[j]] = 1;
			if (i % prime[j] == 0) break;
		}
	}
	for (int i = l; i <= r; i++) {
		if (!st[i]) ans++;
	}
	cout << ans << endl;	
	return 0;
}
