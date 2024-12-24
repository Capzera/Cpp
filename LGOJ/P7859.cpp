#include <bits/stdc++.h>
using namespace std;
long long n, m, a[401][2], ans;
int main() {
	cin >> n >> m;
	for (int i = 0; i < m; i++) cin >> a[i][0] >> a[i][1], a[i][0]--, a[i][1]--;
	for (int i = 1; i < 1 << n; i++) {
		for (int j = 0; j < m; j++) {
			if (i & 1 << a[j][0] && i & 1 << a[j][1]) {
				ans++;
				break;
			}
		}
	}
	cout << (1 << n) - ans;
	return 0;
}
