#include <bits/stdc++.h>
using namespace std;
map<string, string> pa;
string find(string x) {
	return x == pa[x] ? x : pa[x] = find(pa[x]);
}
string sn, fa;
char ch;
int main() {
	while (cin >> ch, ch != '$') {
		cin >> sn;
		if (ch == '#') {
			fa = sn;
			if (!pa.count(sn)) pa[sn] = sn;
		} else if (ch == '+') {
			pa[sn] = fa;
		} else {
			cout << sn << " " << find(sn) << '\n';
		}
	}
	return 0;
}
