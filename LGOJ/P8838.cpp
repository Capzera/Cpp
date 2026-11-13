#include <bits/stdc++.h>
using namespace std;
int n, k, bucket[7], a[7], num[7];
string mn = "9999999", ans;
void dfs(int i) {
	if (i == k + 1) {
		mn = min(mn, ans);
		return;
	}
	for (int j = 1; j <= n; j++) {
		if (bucket[j] || bucket[j] + num[i] > a[j]) continue;
		bucket[j] += a[i];
		ans += j + '0';
		dfs(i + 1);
		bucket[j] -= a[i];
		ans.pop_back();
	}	
}
int main() {
	cin >> n >> k;
	for (int i = 1; i <= n; i++) cin >> a[i];
	for (int i = 1; i <= k; i++) cin >> num[i];
	dfs(1);
	if (mn == "9999999") return puts("-1"), 0;
	for (auto& x : mn) cout << x << " ";
	return 0;
}

