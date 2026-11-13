#include <bits/stdc++.h>
using namespace std;
int n, m, x;
string op;
int main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	cin >> n;
	vector<int> a;
	for (int i = 1; i <= n; i++) {
		cin >> x;
		a.push_back(x);
	}
	sort(a.begin(), a.end());
	cin >> m;
	while (m--) {
		cin >> op;
		if (op == "add") {
			cin >> x;
			int j = lower_bound(a.begin(), a.end(), x) - a.begin();
			a.insert(a.begin() + j, x);
		} else {
			cout << a[(a.size() - 1) / 2] << '\n';
		}
	}
	return 0;
}
