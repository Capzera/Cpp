#include <bits/stdc++.h>
using namespace std;
int n, k, l, j = 1, ans = INT_MAX;
struct ball {
	int id, type;
	bool operator < (const struct ball& rhs) {
		return id < rhs.id;
	}
} a[1000010];
unordered_map<int, int> mp;
int main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	cin >> n >> k;
	for (int i = 1; i <= k; i++) {
		cin >> l;
		while (l--) {
			cin >> a[j].id;
			a[j++].type = i;
		}
	}
	sort(a + 1, a + n + 1);
	for (int i = 1, j = 1; i <= n; i++) {
		mp[a[i].type]++;
		if (mp.size() < k) continue;
		while (mp[a[j].type] > 1) {
			mp[a[j++].type]--;
		}
		ans = min(ans, a[i].id - a[j].id);
	}
	cout << ans;
	return 0;
}
