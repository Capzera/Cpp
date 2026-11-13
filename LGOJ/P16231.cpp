#include <bits/stdc++.h>
using namespace std;
const int N = 1e6 + 10;
string s, s1, s2, tmp;
unordered_map<string, int> mp;
vector<int> dir = {-1};
int q, cnt, l[N], r[N];
void init(string s) {
	s += ",";
	for (auto& x : s) {
		if (!isalpha(x)) {
			if (tmp.empty()) continue;
			mp[tmp] = ++cnt;
			tmp = "";
		} else tmp += x;	
	}
	for (auto& x : s) {
		if (isalpha(x)) {
			if (tmp.empty()) continue;
			dir.push_back(tmp == "=>" ? 1 : tmp == "<=" ? 2 : 3);
			tmp = "";
		} else tmp += x;
	}
	for (int i = 1, cnt = 0; i < dir.size(); i++) {
		l[i] = i - cnt;
		cnt = dir[i] == 1 ? 0 : cnt + 1;
	}
	for (int i = dir.size() - 1; i > 0; i--) {
		r[i] = i + cnt;
		cnt = dir[i - 1] == 2 ? 0 : cnt + 1;
	}
}
int main() {
	cin >> s;
	init(s);
	cin >> q;
	while (q--) {
		cin >> s1 >> s2;
		int x = mp[s1], y = mp[s2];
		if (!x || !y) {
			cout << (s1 == s2 ? "Yes\n" : "No\n");
			continue;
		}
		cout << (l[x] <= y && y <= r[x]? "Yes\n" : "No\n");
	}
	return 0;
}
