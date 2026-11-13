#include <bits/stdc++.h>
using namespace std;
int n, cnt;
vector<vector<int> > ans;
vector<int> a(10);
void dfs(int i, int sum) {
	if (i == 10) {
		if (sum != n) return;
		cnt++;
		ans.push_back(a);
		return;
	}
	for (int j = 1; j <= 3; j++) {
		if (sum + j > n) return;
		a[i] = j;
		dfs(i + 1, sum + j);
	}
}
int main() {
	cin >> n;
	dfs(0, 0);
	cout << cnt << endl;
	for (auto& x : ans) {
		for (auto& y : x) cout << y << " ";
		cout << endl;
	}
	return 0;
}
