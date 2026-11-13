#include <bits/stdc++.h>
using namespace std;
int t;
string s1, s2;
bool isPalin(string s) {
	string t = s;
	reverse(s.begin(), s.end());
	return s == t;
}
bool solve() {
	cin >> s2;
	int n = s2.size();
	string s1;
	for (int i = 0; i < n; i++) {
		s1 += s2[0];
		s2 = s2.substr(1);
		if (s1.size() < 2 || s2.size() < 2) continue;
		if (isPalin(s1) && isPalin(s2)) return 1;
	}
	return 0;
}
int main() {
	cin >> t;
	while (t--) {
		cout << (solve() ? "Yes\n" : "No\n");
	}
	return 0;
}
