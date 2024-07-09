#include <bits/stdc++.h>
using namespace std;
string s;
long long n, ans;
int main() {
	cin >> n >> s;
	s = s[0] + s + s.back();
	vector<int> l(n + 1);
	for (int i = 1, cnt = 0; i <= n; i++) {
		if (s[i - 1] == s[i]) cnt++;
		else l[i] = cnt, cnt = 1;
	}
	for (int i = n, r = 0, cnt = 0; i >= 1; i--) {
		if (s[i] == s[i + 1]) r = 0, cnt++;
		else r = cnt, cnt = 1;
		ans += max(l[i] - 1, 0) + max(r - 1, 0) + 1ll * l[i] * r;
	}
	cout << ans;
	return 0;
}
