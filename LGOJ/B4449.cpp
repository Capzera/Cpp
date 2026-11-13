#include <bits/stdc++.h>
using namespace std;

string s;

char solve() {
	cin >> s;
	if (s.size() < 8) return 0;
	int f1 = 0, f2 = 0;
	for (auto& x : s) {
		if (isdigit(x)) f2 = 1;
		if (isupper(x)) f1 = 1;
	}
	return f1 && f2;
}

int main() {
	int t;
	cin >> t;
	while (t--) cout << (solve() ? "Y\n" : "N\n");
	return 0;
}
