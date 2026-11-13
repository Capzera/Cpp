#include <bits/stdc++.h>
using namespace std;
const int N = 1e6 + 10;
string s, s1, s2, tmp;
unordered_map<string, int> mp;
int q, cnt, idx, l[N], r[N], dir[N];
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
			if (tmp == "") continue;
			dir[++idx] = tmp == "=>" ? 1 : tmp == "<=" ? 2 : 3;
			tmp = "";
		} else tmp += x;
	}
	for (int i = 1, cnt = 0; i <= idx + 1; i++) {
		l[i] = i - cnt;
		cnt = dir[i] == 1 ? 0 : cnt + 1;
	}
	for (int i = idx + 1, cnt = 0 ; i; i--) {
		r[i] = i + cnt;
		cnt = dir[i - 1] == 2 ? 0 : cnt + 1;
	}
}
int main() {
	cin >> s;
	init(s);
	scanf("%d", &q);
	while (q--) {
		cin >> s1 >> s2;
		int x = mp[s1], y = mp[s2];
		if (!x || !y) {
			puts(s1 == s2 ? "Yes" : "No");
			continue;
		}
		puts(l[x] <= y && y <= r[x] ? "Yes" : "No");
	}

	return 0;
}
