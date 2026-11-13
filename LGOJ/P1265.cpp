#include <bits/stdc++.h>
using namespace std;
struct point {
	int x, y;
} a[5010];
double distance(point a, point b) {
	return sqrt(1.0 * pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}
int n, vis[5010];
double ans, dis[5010];
void Prim() {
	memset(dis, 0x7f, sizeof(dis));
	int u = 1;
	dis[u] = 0;
	for (int i = 1; i < n; i++) { // 轮次，总共执行n - 1轮
		for (int v = 1; v <= n; v++) dis[v] = min(dis[v], distance(a[u], a[v]));
		vis[u] = 1, u = -1;
		double mn = DBL_MAX;
		for (int v = 1; v <= n; v++) {
			if (vis[v]) continue;
			if (dis[v] < mn) {
				mn = dis[v];
				u = v;
			}
		}
		ans += mn;
	}
}
int main() {
	cin >> n;
	for (int i = 1; i <= n; i++) {
		cin >> a[i].x >> a[i].y;
	}
	Prim();
	printf("%.2lf", ans);
	return 0;
}
