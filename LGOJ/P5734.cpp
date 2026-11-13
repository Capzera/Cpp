#include <bits/stdc++.h>
using namespace std;
int q, op, a, b;
string s, p;
int main() {
	cin >> q >> s;
	while (q--) {
		cin >> op;
		if (op == 1) cin >> p, s += p;
		if (op == 2) cin >> a >> b, s = s.substr(a, b);
		if (op == 3) cin >> a >> p, s.insert(a, p);
		if (op == 4) cin >> p, cout << int(s.find(p)) << endl;
		else cout << s << endl;
	}
	return 0;
}
