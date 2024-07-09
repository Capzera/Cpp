#include <bits/stdc++.h>
using namespace std;
int main() {
	string s;
	cin >> s;
	int n = s.size(), mx = 0, mn = INT_MAX;
	vector<int> cnt(26);
	for (auto& ch : s) cnt[ch - 'a']++;
	for (auto& x : cnt) {
		if (!x) continue;
		mx = max(mx, x);
		mn = min(mn, x);
	}
	if ((mx - mn) % 2 == 0) {
		cout << "Lucky" << endl << mx - mn;
	} else cout << "No" << endl << "0";
	return 0;
}
