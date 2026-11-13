#include <bits/stdc++.h>
using namespace std;
int t, mn, x, s, ans;
char op;
vector<int> a;
int main() {
	cin >> t >> mn;
	while (t--) {
		cin >> op >> x;
		if (op == 'I') {
			if (x < mn) continue;
			a.insert(lower_bound(a.begin(), a.end(), x), x);
		}
		if (op == 'A') {
			for (auto& ele : a) ele += x;
		}
		if (op == 'S') {
			int n = a.size();
			for (int i = 0; i < n; i++) {
				a[i] -= x;
				if (a[i] < mn) {
					a.erase(lower_bound(a.begin(), a.end(), a[i]));
					i--, n--, ans++;
				}
			}
		}
		if (op == 'F') {
			if (x > a.size()) puts("-1");
			else cout << a[a.size() - x] << endl;
		}
	}
	cout << ans << endl;
	return 0;
}

