#include <iostream>
#include <cmath>
#include <climits>
using namespace std;
int cnt[26];
int main() {
	string s;
	cin >> s;
	int n = s.size();
	int mx = 0, mn = INT_MAX;
	for (int i = 0; i < n; i++) {
		cnt[s[i] - 'a']++;
	}
	for (int i = 0; i < 26; i++) {
		if (cnt[i] == 0) continue;
		mx = max(mx, cnt[i]);
		mn = min(mn, cnt[i]);
	}
	if ((mx - mn) % 2 == 0) {
		cout << "Lucky" << endl << mx - mn;
	} else cout << "No" << endl << "0";
	return 0;
}
