#include <bits/stdc++.h>
using namespace std;
long long n, x, y;
unordered_set<long long> ans;
int main() {
	cin >> n >> x;
	while (n--) {
		cin >> x >> y;
		if (y) ans.insert(x);
	}
	cout << ans.size();
	return 0;
}
