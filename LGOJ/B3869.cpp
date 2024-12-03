#include <bits/stdc++.h>
using namespace std;
long long t, k;
string s;
void solve() {
	cin >> k >> s;
	long long ans = 0, j = 1;
	reverse(s.begin(), s.end());
	for (auto& x : s) {
		ans += j * (x < 'A' ? x - '0' : x - 'A' + 10);
		j *= k;
	}
	cout << ans << endl;
}
int main() {
	cin >> t;
	while (t--) {
		solve();
	}
	return 0;
}
