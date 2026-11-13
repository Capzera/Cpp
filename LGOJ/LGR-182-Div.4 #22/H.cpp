#include <bits/stdc++.h>
using namespace std;
int main() {
	string s;
	cin >> s;
	int n = s.size(), ans = 0;
	for (int i = 0; i < n; i++) {
		vector<int> mp(26);
		for (int j = i; j < n; j++) {
			mp[s[j] - 'a']++;
			int mx = *max_element(mp.begin(), mp.end());
			if (mx <= (j - i + 1) / 2) ans++;
		}
	}
	cout << ans << endl;
	return 0;
}
