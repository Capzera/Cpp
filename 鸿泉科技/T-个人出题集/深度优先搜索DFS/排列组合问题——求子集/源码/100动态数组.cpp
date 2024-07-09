#include <bits/stdc++.h>
using namespace std;
vector<int> ans;
int n;
void dfs(int now) {
	for (int i = now; i <= n; i++) {
		ans.push_back(i);
		for (int i = 0; i < ans.size(); i++) {
			cout << ans[i] << " ";
		}
		cout << endl;
		dfs(i + 1);
		ans.pop_back();
	}
}
int main() {
	cin >> n;
	dfs(1);
	return 0;
}
w
