#include <bits/stdc++.h>
using namespace std;
const int N = 5e5 + 10;
int n, m, ans;
long long x, a[N];
unordered_set<long long> mp;
int main() {
	cin >> n >> m;
	for (int i = 1; i <= n; i++) scanf("%lld", &a[i]);
	for (int i = 1; i <= m; i++) {
		scanf("%lld", &x);
		mp.insert(x);
	}
	a[n + 1] = x;
	for (int i = 1; i <= n; i++) {
		if (!mp.count(a[i]) && mp.count(a[i + 1])) ans++;
	}
	cout << ans;
	return 0;
}
