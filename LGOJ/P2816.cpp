#include <bits/stdc++.h>
using namespace std;
const int N = 5e3 + 10;
int n, a[N];
vector<vector<int> > ans;
int main() {
	cin >> n;
	for (int i = 1; i <= n; i++) cin >> a[i];
	sort(a + 1, a + n + 1);
	ans.push_back({a[1]});
	for (int i = 2; i <= n; i++) {
		int f = 0;
		for (auto& vec : ans) {
			if (a[i] >= vec.size()) {
				vec.push_back(a[i]);
				f = 1;
				break;
			}
		}
		if (!f) ans.push_back({a[i]});
	}
	cout << ans.size();
	return 0;
}

