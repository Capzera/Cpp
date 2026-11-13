#include <bits/stdc++.h>
using namespace std;
int n, x, d, val, pro = 1;
map<vector<int>, int> mp, ans;
int main() {
	cin >> n >> x;
	vector<int> v(n);
	for (int i = 0; i < n; i++) {
		cin >> d;
		pro *= d;
	}
	for (int i = 0; i < pro; i++) {
		for (auto& x : v) cin >> x;
		cin >> val;
		mp[v] = val;
	}
	for (auto&& [vec, val] : mp) {
		vector<int> sum;
		for (int i = 0; i < n; i++) {
			if (i == x) continue;
			sum.push_back(vec[i]);
		}
		ans[sum] += val;
	}
	for (auto&& [x, y] : ans) {
		for (auto& xx : x) cout << xx << " ";
		cout << y << endl;
	}
	return 0;
}
