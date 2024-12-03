#include <bits/stdc++.h>
using namespace std;
long long n, x, cnt;
int main() {
	cin >> n;
	vector<int> ans;
	for (int i = 1, j = 1; i <= n; i++) {
		cin >> x;
		if (x < 0) cnt++;
		if (cnt == 3) {
			ans.push_back(i - j);
			cnt = 1;
			j = i;
		}
		if (i == n && cnt < 3) ans.push_back(n - j + 1);
	}
	cout << ans.size() << endl;
	for (auto& x : ans) cout << x << " ";
	return 0;
}
