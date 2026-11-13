#include <bits/stdc++.h>
using namespace std;
const int N = 1e5 + 10;
struct node { // 抽象，将奶牛抽象为一个二元组 + 编号
	long long x, y, id;
} a[N];
bool cmp1(node x, node y) {
	if (x.x != y.x) return x.x < y.x;
	return x.y < y.y;
}
bool cmp2(node x, node y) {
	if (x.y != y.y) return x.y < y.y;
	return x.x < y.x;
}
long long n, ans = LLONG_MAX;
long long solve(int x, int y, int z) {
	long long xmx = 0, xmn = 50000, ymx = 0, ymn = 50000;
	for (int i = 1; i <= n; i++) {
		if (a[i].id == x || a[i].id == y || a[i].id == z) continue;
		xmx = max(xmx, a[i].x);
		xmn = min(xmn, a[i].x);
		ymx = max(ymx, a[i].y);
		ymn = min(ymn, a[i].y);
	}
	return (xmx - xmn) * (ymx - ymn);
}
int main() {
	vector<int> del;
	cin >> n;
	// 输入
	for (int i = 1; i <= n; i++) {
		cin >> a[i].x >> a[i].y;
		a[i].id = i;
	}
	// 按照横坐标排序
	sort(a + 1, a + n + 1, cmp1);
	for (int i = 0; i < 3; i++) {
		del.push_back(a[i + 1].id); // 按照x排序后小3id
		del.push_back(a[n - i].id); // 按照x排序后大3id
	}
	// 按照纵坐标排序
	sort(a + 1, a + n + 1, cmp2);
	for (int i = 0; i < 3; i++) {
		del.push_back(a[i + 1].id); // 按照y排序后小3id
		del.push_back(a[n - i].id); // 按照y排序后大3id
	}
	int m = del.size();
	for (int i = 0; i < m - 2; i++) {
		for (int j = i + 1; j < m - 1; j++) {
			for (int k = j + 1; k < m; k++) {
				ans = min(ans, solve(del[i], del[j], del[k]));
			}
		}
	}
	cout << ans;
	return 0;
}
