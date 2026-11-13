#include <bits/stdc++.h>
using namespace std;
int n, x, y, loc = -1, mn = INT_MAX;
int main() {
	cin >> n;
	vector<int> a(n), b(n);
	for (auto& x : a) cin >> x;
	for (auto& x : b) cin >> x;
	cin >> x >> y;
	for (int i = 0; i < n; i++) {
		vector<int> f = {a[i] ^ 1, b[i] ^ 1};
		if (f[0] || f[1]) {
			int dis = abs(i + 1 - y);
			if (dis > mn) continue;
			mn = abs(i + 1 - y);
			loc = f[x] ? x : x ^ 1;
		}
	}
	cout << loc;
	if (loc != -1) cout << " " << mn;
	return 0;
}
