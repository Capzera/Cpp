#include <bits/stdc++.h>
using namespace std;
long long n, dp[501][501];
char a[501];
string str;
int main() {
	cin >> n;
	for (int i = 1; i <= n; i++) cin >> a[i]; // 输入数组
	scanf("%s", a + 1);
	for (int i = 1; i <= n; i++) dp[i][i] = 1; // dp边界
	for (int len = 2; len <= n; len++) {
		for (int i = 1; i <= n - len + 1; i++) {
			int j = i + len - 1;
			if (a[i] == a[j]) {
				dp[i][j] = min(dp[i + 1][j], dp[i][j - 1]);
				continue;
			}
			dp[i][j] = INT_MAX;
			for (int k = i; k < j; k++) {
				dp[i][j] = min(dp[i][j], dp[i][k] + dp[k + 1][j]);
			}
		}
	}
	cout << dp[1][n];
	return 0;
}

dp[i][j]  [i..j]能和目标匹配的最小修改次数
dp[i][i] = 1;  dp[1][n]

for (int len = 2; len <= n; len++) {
	for (int i = 1; i + len - 1 <= n; i++) {
		int j = i + len - 1;
	}
}


for (枚举区间长度 2 to) {
	for (枚举起点) {
		计算终点
		[i..j] len
		if (s[i] == s[j]) dp[i][j] = min(dp[i+1..j], dp[i..j-1]);
		else {
			// 两段的符号不等，无法在外部框架通过一笔画将大问题化成小问题
			dp[i][j] = INT_MAX;
			for (int k = i; k < j; k++) {
				dp[i][j] = min(dp[i][j], dp[i][k] + dp[k + 1][j]);
			}
		}
	}
	
}

中心往两端扩展
dp[1][1] dp[2][2] dp[3][3]


dp[min(n, j + w[i])] = max(dp[min(n, j + w[i])], dp[j] + v[j]);


设dp[i] 的含义是：在第i关能获得的分数最大值
dp[i] = max(dp[i], dp[i - a[i]] + b[j]);

