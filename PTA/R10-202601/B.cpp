#include <bits/stdc++.h>
using namespace std;
int t, n;
string s;
int solve() {
	cin >> s;
	n = s.size();
	int L = 0, R = 0;
	for (auto& x : s) x == 'L' ? L++ : R++;
	for (int i = 0, j = 0; i < n; i = (i + 1) % n) {
		if (!L) return 0;
		if (!R) return 1;
		if (s[i] == 'L') {
			while (s[j] != 'R') j = (j + 1) % n;
			R--;
		}
		if (s[i] == 'R') {
			while (s[j] != 'L') j = (j + 1) % n;
			L--;
		}
		s[j] = '#';
	}
}
int main() {
	cin >> t;
	while (t--) cout << (solve() ? "L\n" : "R\n");
	return 0;
}
