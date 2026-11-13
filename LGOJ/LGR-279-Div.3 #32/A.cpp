#include <bits/stdc++.h>
using namespace std;
long long t, n, base[63];
bool check(string s) {
	string t = s;
	reverse(t.begin(), t.end());
	return s == t;
}
int solve() {
	cin >> n;
	for (int i = 0; i <= 63; i++) {
		if (base[i] > n) break;
		long long rem = n - base[i];
		if (check(to_string(rem))) return 1;
	}
	return 0;
}
int main() {
	base[0] = 1;
	for (int i = 1; i <= 63; i++) base[i] = base[i - 1] << 1;
	cin >> t;
	while (t--) cout << (solve() ? "Yes\n" : "No\n");
	return 0;
}
