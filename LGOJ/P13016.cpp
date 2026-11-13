#include <bits/stdc++.h>
using namespace std;
int fac(int x) {
	if (x == 1) return 0;
	if (x & 1 ^ 1) return x >> 1;
	for (int i = 3; i * i <= x; i += 2) {
		if (x % i == 0) return x / i;
	}
	return 1; // prime
}
vector<int> build(int x) {
	vector<int> ans;
	while (x) {
		ans.push_back(x);
		x = fac(x);
	}
	return ans;
}
int t, x, y;
int solve() {
	scanf("%d %d", &x, &y);
	vector<int> a = build(x), b = build(y);
	int l = a.size() - 1, r = b.size() - 1;
	while (l >= 0 && r >= 0 && a[l] == b[r]) l--, r--;
	return l + r + 2;
}

int main() {
	cin >> t;
	while (t--) cout << solve() << '\n';
	return 0;
}
