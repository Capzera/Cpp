#include <bits/stdc++.h>
using namespace std;
int main() {
	int n, k;
	cin >> n >> k;
	vector<int> ans(k);
	function<void(int, int)> dfs = [&](int index, int last) -> void {
		if (index == k) {
			for (auto& x : ans) cout << x << " ";
			cout << endl;
			return;
		}
		for (int i = last; i <= n; i++) {
			ans[index] = i;
			dfs(index + 1, i + 1);
		}
	};
	dfs(0, 1);
	return 0;
}
