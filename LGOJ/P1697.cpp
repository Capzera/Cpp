#include<bits/stdc++.h>
using namespace std;
int n, ans, a[101][2];
int main() {
	cin >> n;
	for (int i = 1; i <= n; i++) {
		cin >> a[i][0] >> a[i][1];
	}
	for (int i = 1; i <= n; i++) {
		vector<int> vis(1001);
		for (int j = 1; j <= n; j++) {
			if (j == i) continue;
			for (int k = a[j][0]; k < a[j][1]; k++) vis[k] = 1;
		}
		int cnt = accumulate(vis.begin(), vis.end(), 0);
		ans = max(ans, cnt);
	}
	cout << ans;
	return 0;
}
