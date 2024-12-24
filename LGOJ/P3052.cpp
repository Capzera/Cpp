#include <bits/stdc++.h>
using namespace std;
long long n, v, bucket[18], w[18];
bool dfs(int i, int x) { // 分组不超过x
	if (i == n) return 1; // 如果所有元素全存进去了
	for (int j = 0; j < x; j++) { // 遍历桶
		if (j && bucket[j] == bucket[j - 1]) continue; // 去除重复情况
		if (bucket[j] + w[i] <= v) { // 如果第i个元素可以放进第j个桶中
			bucket[j] += w[i];
			if (dfs(i + 1, x)) return 1;
			bucket[j] -= w[i];
		}
	}
	return 0;
}
int main() {
	cin >> n >> v;
	for (int i = 0; i < n; i++) cin >> w[i];
	sort(w, w + n, greater<long long>());
	int l = 1, r = n + 1;
	while (l < r) {
		int mid = l + (r - l >> 1);
		memset(bucket, 0, sizeof(bucket));
		if (dfs(0, mid)) r = mid;
		else l = mid + 1;
	}
	cout << r;
	return 0;
}
