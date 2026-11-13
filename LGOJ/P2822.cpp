#include <bits/stdc++.h>
using namespace std;
const int N = 2e3 + 1;
long long t, k, n, m, C[N][N], sum[N][N];
void init() {
	C[0][0] = 1;
	for (int i = 1; i < N; i++) {
		C[i][0] = 1;
		for (int j = 1; j <= i; j++) {
			C[i][j] = (C[i - 1][j] + C[i - 1][j - 1]) % k;
			sum[i][j] =sum[i - 1][j] + sum[i][j - 1] - sum[i - 1][j - 1] + (!C[i][j]);
		}
		sum[i][i + 1] = sum[i][i];
	}
}
long long solve() {
	scanf("%lld%lld", &n, &m);
	return sum[n][min(n, m)];
}
int main() {
	cin >> t >> k;
	init();
	while (t--) cout << solve() << '\n';
	return 0;
}
