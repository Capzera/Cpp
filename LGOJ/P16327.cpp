#include <bits/stdc++.h>
using namespace std;
const int N = 5e5 + 10;
inline int read() {
	int ans = 0;
	char ch = getchar();
	while (!isdigit(ch)) ch = getchar();
	while (isdigit(ch)) {
		ans = ans * 10 + ch - '0';
		ch = getchar();
	}
	return ans;
}
int n, q, lt, rt, l[N], r[N];
long long ans;
unordered_map<int, int> mp;
int main() {
	n = read(), q = read();
	for (int i = 1; i <= n; i++) l[i] = read(), r[i] = read();
	sort(l + 1, l + n + 1), sort(r + 1, r + n + 1);
	ans = n;
	while (q--) {
		lt = read(), rt = read();
		if (!mp[lt - 1]) {
			mp[lt - 1] = 1;
			ans += (lower_bound(l + 1, l + n + 1, lt) - l) - (lower_bound(r + 1, r + n + 1, lt) - r);
		}
		if (!mp[rt]) {
			mp[rt] = 1;
			ans += (lower_bound(l + 1, l + n + 1, rt + 1) - l) - (lower_bound(r + 1, r + n + 1, rt + 1) - r);
		}
		cout << ans << '\n';
	}
	return 0;
}
