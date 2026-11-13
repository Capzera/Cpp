#include <bits/stdc++.h>
using namespace std;
int n, ans = 2, a[1000010];
int main() {
	cin >> n;
	for (int i = 1; i <= n; i++) cin >> a[i];
	for (int i = n; i > 0; i--) a[i] -= a[i - 1];
	for (int i = 2, cnt = 0; i <= n; i++) {
		if (a[i] == a[i - 1]) cnt++;
		else {
			ans = max(ans, cnt);
			cnt = 1;
		}
	}
	cout << ans + 1;
	return 0;
}
/*
O(n^3)
枚举宽度，枚举起点，检验等差，找到答案
O(n^2)
枚举起点，向后延伸，每次延伸的时候对答案更新
O(n)
要求最长等差数列
求最长连续相等数组
1 3 4 5 7 9
1 2 1 1 2 2
 */
