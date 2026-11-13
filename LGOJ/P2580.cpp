#include <bits/stdc++.h>
using namespace std;
set<string> mp1, mp2;
int n, m;
string s;
int main() {
	cin >> n;
	for (int i = 1; i <= n; i++) {
		cin >> s;
		mp1.insert(s);
	}
	cin >> m;
	while (m--) {
		cin >> s;
		if (!mp1.count(s)) cout << "WRONG\n";
		else cout << (mp2.count(s) ? "REPEAT\n" : "OK\n");
		mp2.insert(s);
	}
	return 0;
}
