#include <bits/stdc++.h>
using namespace std;
int n, x, sum, ans;
map<int, int> mp;
int main() {
	cin >> n;
	mp[0] = 0; 
	for (int i = 1; i <= n; i++) {
		cin >> x;
		if (!x) x = -1;
		sum += x;
		if (mp.count(sum)) {
			ans = max(ans, i - mp[sum]);
		}
		else mp[sum] = i;
	}
	cout << ans;
	return 0;
}
/*
-1 1 -1 -1 -1 1 1 0 0

*/
