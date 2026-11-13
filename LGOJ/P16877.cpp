#include <bits/stdc++.h>
using namespace std;
int t, n;
string s;
string solve() {
	cin >> n >> s;
	int up = 0, lo = 0, di = 0, ca = 0;
	for (auto& ch : s) {
		if (isupper(ch)) up = 1;
		if (islower(ch)) lo = 1;
		if (isdigit(ch)) di = 1;
		if (ch == '#' || ch == '@' || ch == '*' || ch == '&') ca = 1;
	}
	if (!up) s += 'A';
	if (!lo) s += 'a';
	if (!di) s += '1';
	if (!ca) s += '#';
	while (s.size() < 7) s += '1';
	return s;
}
int main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	cin >> t;
	for (int i = 1; i <= t; i++) {
		cout << "Case #" << i << ": " << solve() << '\n';
	}
	return 0;
}
