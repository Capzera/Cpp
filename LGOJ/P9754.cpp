#include <bits/stdc++.h>
#define pss pair<string, string>
using namespace std;
const int N = 1e5 + 5;
long long op, k, ad, add;
string s, t, name, ans, type, val;
struct node {
	long long st, size, align;
};
struct St {
	long long size, align;
	vector<pss> mem;
};
map<string, node> a;
map<string, St> stmp;
map<string, long long> mp = {
	{"byte", 1},
	{"short", 2},
	{"int", 4},
	{"long", 8}
};
long long get(long long cur, long long align) {
	return (cur + align - 1) / align * align;
}
void solve() {
	cin >> op;
	if (op == 1) {
		cin >> name >> k;
		long long sz = 0, mx = 0;
		vector<pss> vec;
		while (k--) {
			cin >> type >> val;
			vec.push_back({type, val});
			long long al = mp.count(type) ? mp[type] : stmp[type].align;
			long long size = mp.count(type) ? mp[type] : stmp[type].size;
			mx = max(mx, al);
			sz = get(sz, al) + size;
		}
		sz = get(sz, mx);
		stmp[name] = {sz, mx, vec};
		cout << sz << " " << mx << '\n';
	}
	if (op == 2) {
		cin >> t >> name;
		long long st = 0;
		if (mp.count(t)) {
			st = get(add, mp[t]);
			cout << st << '\n';
			add = st + mp[t];
			a[name] = {st, mp[t], mp[t]};
		} else {
			auto& def = stmp[t];
			st = get(add, def.align);
			cout << st << '\n';
			add = st + def.size;
			a[name] = {st, 0, def.align};
			for (auto& [nt, val] : def.mem) {
				if (!mp.count(nt)) break;
				long long sz = mp[nt];
				st = get(st, sz);
				a[name + "." + val] = {st, sz, sz};
				st += sz;
			}
		}
	}
	if (op == 3) {
		cin >> s;
		cout << a[s].st << '\n';
	}
	if (op == 4) {
		cin >> ad;
		ans = "";
		for (auto& [name, e] : a) {
			if (e.st <= ad && ad < e.st + e.size) {
				ans = name;
				break;
			}
		}
		if (ans == "") cout << "ERR\n";
		else cout << ans << '\n';
	}
}

int main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	int T = 1;
	cin >> T;
	while (T--) solve();
	return 0;
}
