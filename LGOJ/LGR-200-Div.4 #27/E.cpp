#include <bits/stdc++.h>
using namespace std;
long long n, m, op, a, b, c, vec[101][101];
int main() {
	cin >> n >> m;
	while (m--) {
		cin >> op >> a >> b >> c;
		vec[a][b] += c * (op == 1 ? 1 : -1);
		long long ans = INT_MIN;
		for (int i = 1; i <= n; i++)
			for (int j = 1; j <= n; j++) 
				ans = max(ans, vec[i][j]);
		cout << ans << endl;
	}
	return 0;
}
