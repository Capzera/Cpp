#include <bits/stdc++.h>
using namespace std;
int n, m, x, ans;
unordered_set<int> mp;
int main() {
	cin >> n >> m;
	while (n--) {
		cin >> x;
		mp.insert(x);
	}
	while (m--) {
		cin >> x;
		ans += mp.count(x);
	}
	cout << ans;
	return 0;
}
