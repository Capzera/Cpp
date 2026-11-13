#include <bits/stdc++.h>
using namespace std;
string s, t;
int main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	cin >> s;
	t = s;
	reverse(t.begin(), t.end());
	int n = s.size();
	for (int i = n; i > 0; i--) {
		if (i % 2 == n % 2) cout << s.substr(0, i);
		else cout << t.substr(0, i);
	}
	return 0;
}
