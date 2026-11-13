#include <bits/stdc++.h>
using namespace std;
int k;
int main() {
	cin >> k;
	string ans(26, ' ');
	iota(ans.begin(), ans.end(), 'A');
	for (auto& ch : ans) {
		ch = (ch - 'A' + k) % 26 + 'A';
	}
	cout << ans;
	return 0;
}
