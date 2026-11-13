#include <bits/stdc++.h>
using namespace std;
int m, n, ans, a[401], b[401];
int main() {
	cin >> n >> m;
	for (int i = 0; i < m; i++) {
		cin >> a[i] >> b[i];
		a[i]--, b[i]--;
	}
	for (int i = 1; i < 1 << n; i++) {
		for (int j = 0; j < m; j++) {
			if (i >> a[j] & 1 && i >> b[j] & 1) {
				ans++;
				break;
			}
		}
	}
	cout << (1 << n) - ans;
	return 0;
}
