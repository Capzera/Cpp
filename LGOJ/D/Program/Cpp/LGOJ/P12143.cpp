#include <bits/stdc++.h>
using namespace std;
string s;
long long cnt, ans;
bool chk(int i) {
	return s[i - 1] + 1 != s[i] && s[i - 1] != s[i];
}
int main() {
	cin >> s;
	for (int i = 0, j = 0; i < s.size(); i++) {
		if (i && chk(i)) cnt++;
		while (cnt > 1) {
			if (chk(++j)) cnt--;
		}
		ans += i - j + 1;
	}
	cout << ans;
	return 0;
}
