#include <bits/stdc++.h>
#define int long long
using namespace std;

signed main() {
	int n;
	cin >> n;
	vector<int> ans;
	while (n != 1) {
		ans.push_back(n);
		if (n & 1) n = n * 3 + 1;
		else n >>= 1;
	}
	ans.push_back(1);
	for (int i = ans.size() - 1; i >= 0; i--) {
		cout << ans[i] << " ";
	}
	return 0;
}
