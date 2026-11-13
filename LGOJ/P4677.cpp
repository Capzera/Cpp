#include <bits/stdc++.h>
using namespace std;

int main() {
	
	return 0;
}
/*
dp[i][j] 表示前i个村庄有j个学校的距离最小值
dp[i][j] = min(dp[i][j - 1], dp[k][j - 1] + sum[i] - sum[k])
j <= min(i, m)
k = 1; k < i
dp[0][0] = 0;
*/
