#include <bits/stdc++.h>
using namespace std;
int n, q, l, r, a, b, c;
char ch;
set<int> s;
int main() {
	cin >> n >> q;
	while (n--) {
		cin >> a >> ch >> b >> ch >> c;
		s.insert((c - b) / a);
	}
	vector<int> ans(s.begin(), s.end());
	while (q--) {
		cin >> l >> r;
		cout << upper_bound(ans.begin(), ans.end(), r) - lower_bound(ans.begin(), ans.end(), l) << endl;
	}
	return 0;
}
/*
1 3 6
*/
