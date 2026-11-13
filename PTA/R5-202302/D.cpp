#include <bits/stdc++.h>
using namespace std;
string s;
int k;
int main() {
	cin >> s >> k;
	while (k-- && s.size() > 1) {
		int i = 0;
		for (; i < s.size() - 1; i++) {
			if (s[i] > s[i + 1]) break;
		}
		s.erase(i, 1);
	}
	reverse(s.begin(), s.end());
	while (s.size() > 1 && s.back() == '0') s.pop_back();
	reverse(s.begin(), s.end());
	cout << s;
	return 0;
}
