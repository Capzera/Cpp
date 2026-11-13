#include <bits/stdc++.h>
using namespace std;
const int N = 5e5 + 10;
long long n, m, t, a[N];
long long solve() {
	long long x1, y1, x2, y2; // 必须开局部遍历，否则报错
	cin >> x1 >> y1 >> x2 >> y2;
	long long ans = abs(y1 - y2); // 无论哪一种情况均需要移动 y 轴的路径差
	if (y1 * y2 >= 0) return ans + abs(x1 - x2); // 无需穿墙，曼哈顿距离
	if (x1 > x2) swap(x1, x2); // 保证 x1 <= x2;
	int x = lower_bound(a + 1, a + n + 1, x1) - a; // 求大于等于 x1 第一个检查点横坐标
	if (a[x] <= x2) return ans + abs(x1 - x2); // 在两点横坐标之间有检查点，曼哈顿距离
	// 否则说明 a[x] 的位置肯定大于 x2, 那么a[x - 1] 的位置必然小于 x1
	return ans + min(abs(x1 + x2 - 2 * a[x]), abs(x1 + x2 - 2 * a[x - 1]));
}
int main() {
	cin >> n >> m;
	for (int i = 1; i <= n; i++) cin >> a[i];
	sort(a + 1, a + n + 1);
	while (m--) cout << solve() << '\n';
	return 0;
}
