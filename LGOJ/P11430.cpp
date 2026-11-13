#include <bits/stdc++.h>
using namespace std;
const int N = 5e3 + 10, M = 5e3 + 10;
int n, m, p[N], t[N];
long long ans, dp1[M], dp2[M], o[N];
int main() {
	cin >> n >> m;
	for (int i = 1; i <= n; i++) {
		cin >> p[i] >> t[i] >> o[i];
	}
	for (int i = 1; i <= n; i++) {
		for (int j = 0; j <= m; j++) dp1[j] = dp2[j];
		for (int j = t[i]; j <= m; j++) {
			dp1[j] = max(dp1[j], dp1[j - t[i]] + o[i]);
		}
		for (int j = p[i]; j <= m; j++) {
			dp2[j] = max(dp2[j], dp1[j - p[i]]);
		}
	}
	for (int j = 0; j <= m; j++) ans = max(ans, dp2[j]);
	cout << ans;
	return 0;
}
