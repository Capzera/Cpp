#include <bits/stdc++.h>
using namespace std;
long long n, m, ans;
string s, op;
int main() {
	cin >> n >> m >> op;
	while (m) {
		s += m % 2 + '0';
		m >>= 1;
	}
	reverse(s.begin(), s.end());
	for (auto& ch : op) {
		if (ch == 'L') s += "0";
		if (ch == 'R') s += "1";
		if (ch == 'U' && s.size() > 1) s.pop_back();
	}
	for (long long i = s.size() - 1, j = 1; ~i; i--, j <<= 1) {
		ans += j * (s[i] - '0');
	}
	cout << ans;
	return 0;
}
