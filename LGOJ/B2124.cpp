#include <bits/stdc++.h>
using namespace std;
string s;
bool pd(string s) {
	for (int l = 0, r = s.size() - 1; l < r; l++, r--) {
		if (s[l] != s[r]) return 0;
	}
	return 1;
}
int main() {
	cin >> s;
	if (pd(s)) cout << "yes";
	else cout << "no";
	return 0;
}
