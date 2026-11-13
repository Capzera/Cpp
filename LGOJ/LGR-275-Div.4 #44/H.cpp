#include <bits/stdc++.h>
using namespace std;
int n, m, d, t, ans = -1;
int a[21][21];
int main() {
	cin >> n >> m >> d;
	for (int i = 1; i <= n; i++) 
		for (int j = 1; j <= m; j++) 
			cin >> a[i][j];
	for (int r1 = 1; r1 <= n; r1++) {
		for (int c1 = 1; c1 <= m; c1++) {
			for (int r2 = r1; r2 <= n; r2++) {
				for (int c2 = c1; c2 <= m; c2++) {
					unordered_set<int> mp;
					for (int i = r1; i <= r2; i++) 
						for (int j = c1; j <= c2; j++)
							mp.insert(a[i][j]);
					if (mp.size() != d) continue;
					ans = max(ans, (r2 - r1 + 1) * (c2 - c1 + 1));
				}
			}
		}
	}
	if (ans == -1) return puts("Hrk"), 0;
	cout << ans;
	return 0;
}
