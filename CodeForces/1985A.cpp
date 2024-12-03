#include <bits/stdc++.h>
#define int long long
using namespace std;
void solve() {
	string s1, s2;
	cin >> s1 >> s2;
	swap(s1[0], s2[0]);
	cout << s1 << " " << s2 << endl;
}
signed main() {
	int t;
	cin >> t;
	while (t--) {
		solve();
	}
	return 0;
}
