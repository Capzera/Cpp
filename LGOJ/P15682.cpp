#include <bits/stdc++.h>
using namespace std;
string s(9, ' '), tmp, ans = "CCC";
int main() {
	for (int i = 0; i < 9; i++) cin >> s[i];
	for (int i = 0; i < 9; i++) {
		tmp += s[i];
		for (int j = 0; j < 9; j++) {
			if (max(abs(i / 3 - j / 3), abs(i % 3 - j % 3)) != 1) continue;
			tmp += s[j];
			for (int k = 0; k < 9; k++) {
				if (i == k || k == j) continue;
				if (max(abs(k / 3 - j / 3), abs(k % 3 - j % 3)) != 1) continue;
				tmp += s[k];
				ans = min(ans, tmp);
				tmp.pop_back();
			}
			tmp.pop_back();
		}
		tmp.pop_back();
	}
	cout << ans;
	return 0;
}
