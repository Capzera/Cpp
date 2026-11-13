#include <bits/stdc++.h>
using namespace std;
int n, m;
string s;
int main() {
	cin >> n;
	vector<string> word(n);
	string ans(33, '*');
	for (auto& x : word) cin >> x;
	cin >> s;
	m = s.size();
	for (auto& x : word) {
		if (x.find(s) || x == s) continue;
		ans[x[m] - 61] = x[m];
	}
	for (int i = 1; i <= 32; i++) {
		cout << ans[i];
		if (i % 8 == 0) cout << '\n';
	}
	return 0;
}
