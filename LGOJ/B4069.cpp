#include <bits/stdc++.h>
using namespace std;
int t, n;
bool solve() {
	cin >> n;
	vector<string> a(n);
	string s;
	for (auto& x : a) cin >> x;
	sort(a.begin(), a.end());
	for (auto& x : a) s += x;
	for (int i = 0; i < s.size() - 1; i++) {
		if (s[i] > s[i + 1]) return 0;
	}
	return 1;
}
int main() {
	cin >> t;
	while (t--) {
		cout << solve() << endl;
	}
	return 0;
}
