#include <bits/stdc++.h>
using namespace std;
int t;

int get(int x) {
	int ans = 0;
	while (x) {
		ans += x % 10;
		x /= 10;
	}
	return ans;
}
bool solve() {
	string s;
	cin >> s;
	int x, n = s.size(), ans = 0;
	for (int i = n - 1; i >= 0; i -= 2) {
		x = (s[i] - '0') * 7;
		while (get(x) > 9) x = get(x);
		s[i] = get(x) + '0';
	}
	for (int i = 0; i < n; i++) ans += s[i] - '0';
	return ans % 8 == 0;
}
int main() {
	cin >> t;
	while (t--) {
		cout << (solve() ? "T" : "F") << endl;
	}
	return 0;
}
/*
2 3
4 5
4 7 8
*/
