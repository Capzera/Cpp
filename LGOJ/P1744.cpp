#include <bits/stdc++.h>
using namespace std;
const int N = 103;
int n, m, u, v, s, t, x[N], y[N];
double dis[N][N];
double Euclid(int u, int v) {
	return sqrt(pow(x[u] - x[v], 2) + pow(y[u] - y[v], 2));
}
int main() {
	cin >> n;
	memset(dis, 0x7f, sizeof(dis));
	for (int i = 1; i <= n; i++) dis[i][i] = 0;
	for (int i = 1; i <= n; i++) cin >> x[i] >> y[i];
	cin >> m;
	while (m--) {
		cin >> u >> v;
		dis[u][v] = dis[v][u] = min(dis[u][v], Euclid(u, v));
	}
	for (int k = 1; k <= n; k++)
		for (int i = 1; i <= n; i++)
			for (int j = 1; j <= n; j++)
				dis[i][j] = min(dis[i][j], dis[i][k] + dis[k][j]);
	cin >> s >> t;
	printf("%.2lf", dis[s][t]);
	return 0;
}
