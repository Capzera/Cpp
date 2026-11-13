#include <bits/stdc++.h>
using namespace std;
void solve() {
	string s, word;
	int k, ans = 0;
	cin >> s >> k;
	vector<int> a(k + 1);
	for (int i = 1; i <= k; i++) cin >> a[i]; a.push_back(s.size());
	for (int i = 0; i <= k; i++) {
		word = s.substr(a[i], a[i + 1] - a[i]);
		if (word.size() < 7) continue;
		for (int i = 0; i <= word.size() - 7; i++) {
			if (word.substr(i, 7) != "respect") continue;
			ans++;
			break;
		}
	}
	cout << ans << endl;
}
int main() {
	int t = 1;
	cin >> t;
	while (t--) solve();
	return 0;
}
