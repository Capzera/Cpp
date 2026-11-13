#include <bits/stdc++.h>
using namespace std;
int t, n, m;
string s;
bool solve() {
	cin >> n >> m >> s;
	int cnt = 0, last = 0, st, ed, i = 0, j = m - 1;
	while (i < m && s[i] == '0') i++;
	while (j >= 0 && s[j] == '0') j--;
	st = i, ed = m - j - 1;
	if (i == m) return st <= n; // 全是0
	if (i == j) return st <= n && ed <= n; // 只有1个1
	if (n < max(st, ed)) return 0; // 前缀和后缀大于n
	for (i++; i <= j; i++) {
		if (s[i] == '0') cnt++;
		else {
			if (last && cnt != last || cnt > n) return 0; // 不是第一个间隔并且间隔数不等或者间隔大于n
			last = cnt;
			cnt = 0;
		}
	}
	if (last < max(st, ed)) return 0; // 中间的0的个数小于前后缀
	return 1;
}
int main() {
	cin >> t;
	while (t--) {
		cout << (solve() ? "Yes\n" : "No\n");
	}
	return 0;
}
