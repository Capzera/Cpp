#include <bits/stdc++.h>
using namespace std;
const int N = 1e5 + 10;
int n, k, sz, tgt, a[N];
unordered_map<int, int> mp;
vector<int> ans, pos;
int main() {
	cin >> n >> k;
	for (int i = 1; i <= n; i++) {
		cin >> a[i];
		mp[a[i]]++;
		if (a[i] == a[i - 1]) continue;
		sz++;
		pos.push_back(i); // pos 数组存储差异点下标
	}
	if (sz == k) return puts("0"), 0;
	if (sz > k) {
		tgt = a[pos[k - 1]];
		for (int i = pos[k - 1] + 1; i <= n; i++) ans.push_back(i);
	} else {
		for (int i = 1; i <= n; i++) {
			if (mp[i]) continue;
			tgt = i;
			break;
		}
		int m = k - sz; // 需要再额外分出 m 组出来
		for (int i = 1; i <= n && m; i++) {
			if (a[i] != a[i - 1]) continue;
			if (i < n && a[i] == a[i + 1] && m == 1) continue; // ★★★★★★★★
			ans.push_back(i);
			if (i < n && a[i] == a[i + 1]) {
				m -= 2, i++;
			} else m--;
		}
	}
	cout << "1\n" << ans.size() << " ";
	for (auto& x : ans) cout << x << " ";
	cout << tgt;
	return 0;
}
