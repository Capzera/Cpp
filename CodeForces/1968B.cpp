#include <bits/stdc++.h>
using namespace std;
void solve() {
	int l1, l2, i = 0, j = 0;
	string s1, s2;
	cin >> l1 >> l2 >> s1 >> s2;
	for (; i < l1; i++, j++) {
		while (j < l2 && s1[i] != s2[j]) j++;
		if (j == l2) break;
	}
	cout << i << endl;
}
int main() {
	int t;
	cin >> t;
	while (t--) {
		solve();
	}
	return 0;
}
