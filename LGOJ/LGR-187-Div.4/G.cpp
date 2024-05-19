#include <bits/stdc++.h>
using namespace std;
int main() {
	string s, ans;
	cin >> s;
	for (auto& ch : s) {
		if (isalpha(ch)) ans += ch;
	}
	cout << ans;
	return 0;
}
