#include <bits/stdc++.h>
using namespace std;
struct info {
	int id, cost;
} a[21][21]; // a[i][j] 第i个原件执行第j个工序.id是机器编号.cost是花费
int num[401], step[21], last[21], ans; // step[i] 第i个原件进行到了第step[i] 步
bool vis[21][8001]; // vis[i][j] 第i台机器在时间j处有安排
long long m, n;
int main() {
	cin >> m >> n;
	for (int i = 1; i <= n * m; i++) cin >> num[i];
	for (int i = 1; i <= n; i++)
		for (int j = 1; j <= m; j++) 
			cin >> a[i][j].id;
	for (int i = 1; i <= n; i++)
		for (int j = 1; j <= m; j++) 
			cin >> a[i][j].cost;
	for (int i = 1; i <= n * m; i++) {
		step[num[i]]++;
		int id = a[num[i]][step[num[i]]].id, cost = a[num[i]][step[num[i]]].cost, sum = 0;
		for (int j = last[num[i]] + 1; ; j++) {
			sum = !vis[id][j] ? sum + 1 : 0;
			if (sum < cost) continue;
			for (int k = j - cost + 1; k <= j; k++) vis[id][k] = 1;
			ans = max(ans, j);
			last[num[i]] = j;
			break;
		}
	}
	cout << ans;
	return 0;
}
