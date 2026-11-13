#include <bits/stdc++.h>
#define int long long
using namespace std;
const int N = 1e5 + 5, M = 1e6 + 5;
int n, x, ans, d[N];
vector<int> a;
signed main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	cin >> n;
	for (int i = 2; i <= n; i++) {
		cin >> d[i];
		d[i] += d[i - 1];
	}
	for (int i = 1; i <= n; i++) {
		cin >> x;
		if (!x) a.push_back(i);
	}
	for (int i = 1; i < a.size(); i++) {
		int l = a[i - 1], r = a[i];
		if (r - l == 1) continue;
		int len = (d[r] - d[l]) % (r - l) == 0 ? (d[r] - d[l]) / (r - l) : 0;
		for (int j = l + 1; j < r; j++) {
			ans += d[j] != d[l] + (j - l) * len;
		}
	}
	cout << ans;
	return 0;
}
