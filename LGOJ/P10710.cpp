#include <bits/stdc++.h>
using namespace std;
struct stu {
	int h, c;
	bool operator < (const struct stu& rhs) const {
		return h < rhs.h;
	}
} a[1000001];
int n, m, ans = INT_MAX;
unordered_map<int, int> mp;
int main() {
	cin >> n >> m;
	for (int i = 0; i < n; i++) {
		for (int j = 1; j <= m; j++) {
			cin >> a[i * m + j].h;
			a[i * m + j].c = i;
		}
	}
	sort(a + 1, a + n * m + 1);
	for (int i = 1, j = 1; i <= n * m; i++) {
		mp[a[i].c]++;
		while (j <= i && mp[a[j].c] > 1) {
			mp[a[j++].c]--;
		}
		if (mp.size() == n) ans = min(ans, a[i].h - a[j].h);
	}
	cout << ans;
	return 0;
}
/*
重练了scanf
重练运算符重载
滑窗
二维->一维
map
*/
