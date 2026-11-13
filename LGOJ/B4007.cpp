#include <bits/stdc++.h>
using namespace std;
int n, k, ans;
string s;
int main() {
	cin >> n >> k;
	for (int i = 1; i <= n; i++) {
		s = to_string(i);
		for (auto& ch : s) ans += ch - '0' == k;
	}
	cout << ans;
	return 0;
}
