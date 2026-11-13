#include <bits/stdc++.h>
using namespace std;
int n, x, ans = INT_MAX;
map<int, int> mp;
int main() {
	cin >> n;
	while (n--) {
		cin >> x;
		mp[x]++;
	}
	while (mp.size()) {
		auto&& i = mp.begin(), j = mp.begin();
		--(*i).second;
		int cnt = 1;
		for (++j; j != mp.end() && (*j).first == (*i).first + 1 && (*i).second < (*j).second; i++, j++) {
			cnt++;
			--(*j).second;
		}
		i = mp.begin();
		while (i != mp.end() && (*i).second == 0) {
			mp.erase((*i++).first);
		}
		ans = min(ans, cnt);
	}
	cout << ans;
	return 0;
}
