#include<bits/stdc++.h>
using namespace std;
int n, m, f[10010], sum[10010], w[10010];
int main() {
	cin >> n >> m;
	memset(f, 0x3f, sizeof(f));
	for (int i = 1; i <= n; i++) {
		cin >> w[i];
		sum[i] = sum[i - 1] + w[i];
	}
	f[0] = 0;
	for (int i = 1; i <= n; i++) {
		sum[i] += 2 * m;
	}
	for (int i = 1; i <= n; i++) {
		for (int j = i; j <= n; j++) {
			f[j] = min(f[j], f[j - i] + sum[i]);
		}
	}
	cout << f[n] - m;
	return 0;
}

