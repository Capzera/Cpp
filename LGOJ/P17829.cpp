#include<bits/stdc++.h>
using namespace std;
int n, m, ans;
int main() {
	// freopen("and.in", "r", stdin);
	// freopen("and.out", "w", stdout);
	cin >> n >> m;
	vector<int> a(n);
	for (int& x : a) cin >> x;
	for (int i = 29; ~i; i--) {
		int cnt = 0;
		for (int x : a) cnt += x >> i & 1;
		if (cnt < m) continue;
		ans |= 1 << i;
		vector<int> tmp;
		for (int x : a) if (x >> i & 1) tmp.push_back(x);
		a.swap(tmp);
	}
	cout << ans;
}
