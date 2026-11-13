#include <bits/stdc++.h>
using namespace std;
long long n, m, p, c, cnt, ans = LLONG_MAX;
int main() {
	cin >> n >> m;
	vector<long long> a[n + 1];
	for (int i = 1; i <= m; i++) {
		cin >> p >> c;
		a[p].push_back(c);
	}
	for (int i = 1; i <= n; i++) {
		sort(a[i].begin(), a[i].end());
	}
	for (int x = max((int)a[1].size(), 1); x <= m; x++) { 
		long long cnt = a[1].size(), sum = 0;
		vector<int> tmp;
		for (int i = 2; i <= n; i++) {
			int j = max((int)a[i].size() - x + 1, 0);
			for (int k = 0; k < j; k++) sum += a[i][k];
			cnt += j;
			for (; j < a[i].size(); j++) {
				tmp.push_back(a[i][j]);
			}
		}
		sort(tmp.begin(), tmp.end());
		for (int i = 0; i < x - cnt; i++) {
			sum += tmp[i];
		}
		ans = min(ans, sum);
	}
	cout << ans;
	return 0;
}
