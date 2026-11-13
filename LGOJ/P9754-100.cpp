#include <bits/stdc++.h>
#define pss pair<string, string>
using namespace std;
const int N = 1e5 + 5;
long long op, k, ad, add;
string s, t, name, ans, type, val;
struct node {
	long long st, size, align;
	string type;
};
struct St {
	long long size, align;
	vector<pss> mem;
};
map<string, node> a;
map<string, St> mp = {
	{"byte",  {1, 1, {}}},
	{"short", {2, 2, {}}},
	{"int",   {4, 4, {}}},
	{"long",  {8, 8, {}}}
};
long long get(long long cur, long long align) {
	return (cur + align - 1) / align * align;
}
void solve() {
	cin >> op;
	if (op == 1) {
		cin >> name >> k;
		long long st = 0, mx = 0;
		vector<pss> vec;
		while (k--) {
			cin >> t >> val;
			vec.push_back({type, val});
			long long al = mp[t].align, sz = mp[t].size;
			mx = max(mx, al);
			st = get(st, al) + sz;
		}
		st = get(st, mx);
		mp[name] = {st, mx, vec};
		cout << st << " " << mx << '\n';
	}
	if (op == 2) {
		cin >> t >> name;
		long long al = mp[t].align, sz = mp[t].size;
		long long st = get(add, al);
		cout << st << '\n';
		add = st + sz;
		a[name] = {st, sz, al, t};
	}
	if (op == 3) {
		cin >> s;
		for (auto& c : s) if (c == '.') c = ' ';
		stringstream ss(s);
		ss >> name;
		long long st = a[name].st;
		string cur = a[name].type;
		while (ss >> val) {
			for (auto& [nt, nval] : mp[cur].mem) {
				st = get(st, mp[nt].align);
				if (nval == val) {
					cur = nt;
					break;
				}
				st += mp[nt].size;
			}
		}
		cout << st << '\n';
	}
	if (op == 4) {
		cin >> ad;
		string cur;
		long long st = 0;
		ans = "";
		for (auto& [name, e] : a) {
			if (e.st <= ad && ad < e.st + e.size) {
				ans = name;
				st = e.st;
				cur = e.type;
				break;
			}
		}
		if (ans == "") {cout << "ERR\n"; return;}
		while (!mp[cur].mem.empty()) {
			int flag = 0;
			for (auto& [nt, nval] : mp[cur].mem) {
				st = get(st, mp[nt].align);
				if (st <= ad && ad < st + mp[nt].size) {
					ans += "." + nval;
					cur = nt;
					flag = 1;
					break;
				}
				st += mp[nt].size;
			}
			if (!flag) {cout << "ERR\n"; return;}
		}
		cout << ans << '\n';
	}
}
int main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	int T = 1;
	cin >> T;
	while (T--) solve();
	return 0;
}
