#include <bits/stdc++.h>
#define int long long
using namespace std;
int n, m, k, x, ans;	
signed main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	cin >> n >> m >> k;
	for (int i = 0; i < m; i++) {
		int cnt = 0;
		for (int j = 0; j < n; j++) {
			cin >> x;
			cnt += x == k;
		}
		ans += cnt * 2 >= n;
	}
	cout << (ans * 2 >= m ? "YES" : "NO");
	return 0;
}
