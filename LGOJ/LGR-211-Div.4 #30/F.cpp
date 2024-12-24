#include <bits/stdc++.h>
using namespace std;
long long n, t, p, k, tid[1001], pid[1001], st[1001], mp[101][101], cnt[101], ans[4] = {-1, -1, -1, -1};
int main() {
	cin >> n >> t >> p >> k;
	for (int i = 1; i <= n; i++) {
		cin >> tid[i] >> pid[i] >> st[i];
		if (!st[i]) continue;
		ans[0] = tid[i];
		if (!mp[tid[i]][pid[i]]) {
			if (!cnt[tid[i]]) ans[3] = tid[i];
			cnt[tid[i]]++;
			mp[tid[i]][pid[i]] = 1;
			ans[1] = tid[i];
		}
	}
	for (int i = n; i > 0; i--) {
		if (!st[i]) continue;
		if (cnt[tid[i]] < k) {
			ans[2] = tid[i];
			break;
		}
	}
	for (int i = 0; i < 4; i++) cout << ans[i] << " ";
	return 0;
}
