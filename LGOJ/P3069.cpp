#include <bits/stdc++.h>
using namespace std;
const int N = 1e5 + 10;
unordered_map<int, int> mp;
int n, k, ans, a[N];
int main() {
	cin >> n >> k;
	for (int i = 1; i <= n; i++) cin >> a[i];
	for (int i = 1, j = 1; i <= n; i++) {
		mp[a[i]]++;
		while (j < i && mp.size() > k + 1) {
			--mp[a[j]];
			if (!mp[a[j]]) mp.erase(a[j]);
			j++;
		}
		ans = max(ans, mp[a[i]]);
	}
	cout << ans;
	return 0;
}
