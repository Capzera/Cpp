#include <bits/stdc++.h>
using namespace std;
string s, t;
int p, cnt;
int main() {
	cin >> s;
	int n = s.size();
	cout << n << " ";
	while (--n) cin >> t, s += t;
	for (int i = 0; i < s.size(); i++) {
		if (s[i] != '0' + p) {
			cout << cnt << " ";
			p ^= 1, cnt = 1;
		} else cnt++;
	}
	cout << cnt;
	return 0;
}
