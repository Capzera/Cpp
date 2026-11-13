#include <bits/stdc++.h>
using namespace std;
string s;
int main() {
	cin >> s;
	int n = s.size();
	for (int i = 0; i < n - 1; i++) {
		if (s[i] == '.' && islower(s[i + 1])) s[i + 1] &= -33;
	}
	for (int i = n - 1; ~i; i--) {
		if (s[i] != '.' && isupper(s[i + 1])) s.insert(i + 1, 1, '.');
	}
	s[0] = toupper(s[0]);
	if (s.back() != '.') s += '.';
	cout << s;
	return 0;
}
