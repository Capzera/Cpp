#include <bits/stdc++.h>
using namespace std;
const int N = 4e6 + 10;
long long n, m, slide, l, r, ans, a[N];
int main() {
	cin >> n >> m;
	for (int i = 1; i <= n; i++) cin >> a[i];
	for (int i = 1, j = 1; i <= n; i++) { // i 代表窗口右端点 , j 代表窗口左端点
		// 向窗口内加入一个右端点的i值
		// 对窗口左端点进行抛弃，直到窗口满足题目要求为止，统计答案
		slide += a[i];
		while (j <= i && slide > m) slide -= a[j++];
		if (i > j && slide > ans) {
			ans = slide;
			l = j, r = i;
		}
	}
	cout << l << " " << r << " " << ans;
	return 0;
}
