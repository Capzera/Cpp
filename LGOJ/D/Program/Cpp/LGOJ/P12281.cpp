#include <bits/stdc++.h>
using namespace std;
string ans, op;
int t, m, p;
char ch;
int main() {
	cin >> t;
	while (t--) {
		cin >> op;
		m = op.size();
		if (op == "insert") {
			string s;
			cin >> ch, ch = getchar();
			while (ch != '\"') {
				s += ch;
				ch = getchar();
			}
			ans.insert(p, s);
			p += s.size();
		} else if (op[0] == 'd') {
			int n = stoi(op.substr(1, op.size() - 2));
			if (op.back() == 'h') {
				int st = max(p - n, 0);
				ans.erase(st, p - st);
				p = st;
			} else ans.erase(p, min(n, (int)ans.size() - p));
		} else {
			int n = stoi(op.substr(0, op.size() - 1));
			p = op.back() == 'h' ? max(p - n, 0) : min(p + n, (int)ans.size());
		}
	}
	cout << ans;
	return 0;
}
