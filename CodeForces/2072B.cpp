#include <bits/stdc++.h>
using namespace std;
int t, n;
string s;
long long solve() {
	cin >> n >> s;
	long long c1 = 0, c2 = 0;
	for (auto& ch : s) {
		if (ch == '-') c1++;
		else c2++;
	}
	if (c1 < 2) return 0;
	long long mid = c1 >> 1;
	return 1ll * mid * (c1 - mid) * c2;
}
int main() {
	cin >> t;
	while (t--) {
		cout << solve() << endl;
	}
	return 0;
}
