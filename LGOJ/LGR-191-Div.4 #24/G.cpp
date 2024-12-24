#include <bits/stdc++.h>
using namespace std;
int main() {
	string s1, s2;
	cin >> s1 >> s2;
	int l1 = s1.size(), l2 = s2.size();
	auto check = [&](string s) {
		for (int i = 0; i < l2; i++) {
			if (s[i] == s2[i]) continue;
			if (s[i] == '?' || s2[i] == '?') continue;
			return 0;
		}
		return 1;
	};
	for (int i = 0; i <= l1 - l2; i++) {
		string p = s1.substr(i, l2);
		if (check(p)) cout << i + 1 << " " << i + l2 << endl;
	}
	return 0;
}
