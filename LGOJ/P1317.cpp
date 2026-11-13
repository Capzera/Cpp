#include <bits/stdc++.h>
using namespace std;
	int n, x, ans;
	vector<int> a;
int main() {
	cin >> n;
	while (n--) {
		cin >> x;
		if (a.empty() || x != a.back()) a.push_back(x);
	}
	for (int i = 1; i < a.size() - 1; i++) {
		ans += a[i] < a[i - 1] && a[i] < a[i + 1];
	}
	cout << ans;
	return 0;
}
