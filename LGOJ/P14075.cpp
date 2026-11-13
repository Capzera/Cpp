#include <bits/stdc++.h>
using namespace std;
const int N = 1e5 + 10;
string s;
long long n, a[N], dp[N];
unordered_map<char, int> mp;
int main() {
	cin >> n >> s;
	for (int i = 1; i <= n; i++) cin >> a[i];
	for (int i = 1, j = 1; i <= n; i++) {
		j = max(j, mp[s[i - 1]] + 1);
		for (int k = 1; k <= i - j + 1; k++) {
			dp[i] = max(dp[i], dp[i - k] + a[k]);
		}
		mp[s[i - 1]] = i;
	}
	cout << dp[n];
	return 0;
}
/*
设dp[i] 表示以i结尾的字符串的最优划分得分
*/
