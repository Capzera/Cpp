#include <bits/stdc++.h>
using namespace std;
char a[101][101];
int t, n, m;
bool check(int x, int y) {
	for (int i = 0; i < 4; i++) {
		if (a[x][y + i] == '1' || a[x + 3][y + i] == '1' || a[x + i][y] == '1' || a[x + i][y + 3] == '1') return 0;
	}
	if (a[x + 1][y + 1] == '0' || a[x + 1][y + 2] == '0' || a[x + 2][y + 1] == '0' || a[x + 2][y + 2] == '0') return 0;
	return 1;
}
bool solve() {
	cin >> n >> m;
	for (int i = 1; i <= n; i++)
		for (int j = 1; j <= m; j++)
			cin >> a[i][j];
	for (int i = 1; i <= n - 3; i++) {
		for (int j = 1; j <= m - 3; j++) {
			if (check(i, j)) return 1;
		}
	}
	return 0;
}
int main() {
	cin >> t;
	while (t--) {
		cout << (solve() ? "Yes\n" : "No\n");
	}
	return 0;
}
/*
字符串模拟， map
数位拆分（进制转换）
二维数组
排序
位运算 xxxxx 	

*/
