#include <bits/stdc++.h>
using namespace std;
int n, m, x, tree[100010];
string s;
stack<int> stk;
unordered_map<int, int> mp;
void build(int l, int r, int lo, int hi) {
	if (s[l] != 'x') {
		int x = 0;
		for (int i = l; i <= r; i++) x = x * 10 + s[i] - '0';
		for (int i = lo; i <= hi; i++) tree[i] = x;
		return;
	}
	int i = l + 2, x = 0;
	for (; s[i] != '?'; i++) x = x * 10 + s[i] - '0';
	if (s[l + 1] == '<') {
		build(i + 1, mp[i] - 1, lo, x - 1);
		build(mp[i] + 1, r, x, hi);
	} else {
		build(i + 1, mp[i] - 1, x + 1, hi);
		build(mp[i] + 1, r, lo, x);
	}
}
int main() {
	cin >> n >> m >> s;
	for (int i = 0; i < s.size(); i++) {
		if (s[i] == '?') stk.push(i);
		else if (s[i] == ':') {
			mp[stk.top()] = i;
			stk.pop();
		}
	}
	build(0, s.size() - 1, 1, n);
	while (m--) {
		cin >> x;
		cout << tree[min(n, x)] << endl;
	}
	return 0;
}
