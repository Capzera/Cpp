#include <bits/stdc++.h>
using namespace std;
const int N = 1e5 + 5;
long long op, ad, add;
string s, t, name;
struct node {
	long long st, size;
};
map<string, node> a;
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
		
	}
	if (op == 2) {
		cin >> t >> name;
		long long sz = mp[t];
		long long st = get(add, sz);
		add = st + sz;
		a[name] = {st, sz};
		cout << st << '\n';
	}
	if (op == 3) {
		cin >> s;
		cout << a[s].st << '\n';
	}
	if (op == 4) {
		cin >> ad;
		string ans;
		for (auto& [name, e] : a) {
			if (e.st <= ad && ad < e.st + e.size) {
				ans = name;
				break;
			}
		}
		if (ans == "") {cout << "ERR\n"; return;}
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
