#include <bits/stdc++.h>
using namespace std;
map<string, int> mp, sum;
int n, m, ans1, ans2;
string s, a[100001];
int main() {
	cin >> n;
	for (int i = 1; i <= n; i++) cin >> s, mp[s]++;
	cin >> m;
	for (int i = 1, j = 1; i <= m; i++) {
		cin >> a[i];
		if (mp[a[i]])
			if (++sum[a[i]] == 1) 
				ans1++, ans2 = i - j + 1;
		while (j <= i && (!mp[a[j]] || sum[a[j]] > 1)) {
			if (sum[a[j]] > 1) sum[a[j]]--;
			j++;
		}
		ans2 = min(ans2, i - j + 1);
	}
	cout << ans1 << '\n' << ans2;
	return 0;
}
