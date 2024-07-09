#include <bits/stdc++.h>
using namespace std;
int main() {
	int n, k;
	string s;
	cin >> n >> k >> s;
	for (int i = 0; i < n && k; i++) {
		if (s[i] != '6' && s[i] != '0' && s[i] != '3' && s[i] != '1') continue;
		if (s[i] == '6') s[i] = '9';
		if (s[i] == '1') s[i] = '7';
		if (s[i] == '0' || s[i] == '3') s[i] = '8';
		k--;
	}
	cout << s;
	return 0;
}
