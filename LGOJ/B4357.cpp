#include <bits/stdc++.h>
using namespace std;
int l, r, ans, mp[100001];
int main() {
	cin >> l >> r;
	vector<int> base;
	for (int i = 1; i <= r; i <<= 1) {
		mp[i] = 1;
		base.push_back(i);
	}
	for (int i = l; i <= r; i++) {
		int f = 0;
		for (auto& x : base) {
			if (x > i) break;
			if (!mp[i - x]) continue;
			ans++;
			break;
		}
	}
	cout << ans;
	return 0;
}
