#include <bits/stdc++.h>
using namespace std;
int n, m, cost[51][3], ans = INT_MAX;
char x;
int main() {
	cin >> n >> m;
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= m; j++) {
			cin >> x;
			if (x == 'W') cost[i][0]++;
			if (x == 'B') cost[i][1]++;
			if (x == 'R') cost[i][2]++;
		}
		for (int j = 0; j < 3; j++) cost[i][j] += cost[i - 1][j];
	}
	for (int i = 1; i <= n - 2; i++)
		for (int j = i + 1; j <= n - 1; j++)
			ans = min(ans, cost[i][1] + cost[i][2] + (cost[j][0] + cost[j][2] - cost[i][0] - cost[i][2]) + (cost[n][0] + cost[n][1] - cost[j][0] - cost[j][1]));
	cout << ans;
	return 0;
}
