#include <bits/stdc++.h>
using namespace std;
unordered_set<string> mp;
unordered_map<string, int> name;
string s;
int n, m, ans, id, frag[] = {0, 2000, 1000, 200, 36, 6};
int main() {
	cin >> n >> m;
	while (n--) {
		cin >> s >> id;
		name[s] = id;
	}
	while (m--) {
		cin >> s;
		if (mp.count(s)) ans += frag[name[s]];
		else mp.insert(s);
	}
	cout << ans;
	return 0;
}
