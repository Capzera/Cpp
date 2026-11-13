#include <bits/stdc++.h>
using namespace std;
int n, way;
int ans[14]; // ans[i] 第i个皇后应该放在(i, ans[i])
int col[14];  // 列
int dia1[27]; // 主对角线，i + j为定值
int dia2[26]; // 从对角线，i - j + n 为定值
void dfs(int i) { 
	if (i == n + 1) {
		way++;
		if (way <= 3) {
			for (int j = 1; j <= n; j++) cout << ans[j] << " ";
			cout << endl;
		}
		return;
	}
	for (int j = 1; j <= n; j++) {
		if (col[j] || dia1[i + j] || dia2[i - j + n]) continue;
		col[j] = dia1[i + j] = dia2[i - j + n] = 1;
		ans[i] = j;
		dfs(i + 1);
		col[j] = dia1[i + j] = dia2[i - j + n] = 0;
	}
}
int main() {
	cin >> n;
	dfs(1);
	cout << way;
	return 0;
}
