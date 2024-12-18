#include <bits/stdc++.h>
using namespace std;
int T, n, p[31], num[31], c[31][31];
long long sum[30];
long long C(long long x) {
	return 1ll * x * (x - 1) / 2;
}
void solve() {
	memset(num, 0, sizeof(num));
	cin >> n;
	int i = 30;
	while ((n & p[i]) == 0) i--;
	long long ans = sum[i - 1] + C(n - p[i] + 1) + n - p[i] + 1, cnt = 0;
	for (int j = i - 1; j >= 0; j--) {
		if ((n & p[j]) == 0) continue;
		for (int k = 0; k <= j; k++) num[k + cnt] += c[j][k];
		cnt++;
	}
	num[cnt]++;
	for (int j = 0; j <= i; j++) ans -= 1ll * p[j] * num[j];
	cout << 6ll * ans << endl;
}
int main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	p[0] = c[0][0] = 1;
	for (int i = 1; i <= 30; i++) p[i] = p[i - 1] << 1; // 2幂数
	for (int i = 1; i <= 30; i++) { // 组合数
		c[i][0] = 1;
		for (int j = 1; j <= i; j++) 
			c[i][j] = c[i - 1][j - 1] + c[i - 1][j];
	}
	for (int i = 1; i <= 29; i++) {
		sum[i] = sum[i - 1] + 1ll * C(p[i]) + p[i];
		for (int j = 0; j <= i; j++) {
			sum[i] -= 1ll * c[i][j] * p[j];
		}
	}
	cin >> T;
	while (T--) {
		solve();
	}
	return 0;
}
