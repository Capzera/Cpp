#include <bits/stdc++.h>
using namespace std;
int n, op;
string s, x;
unordered_map<string, string> mp;
int main() {
	cin >> n;
	while (n--) {
		cin >> op;
		if (op == 1) {
			cin >> s >> x;
			mp[s] = x;
			cout << "OK\n";
		}
		if (op == 2) {
			cin >> s;
			cout << (mp.count(s) ? mp[s] : "Not found") << endl;
		}
		if (op == 3) {
			cin >> s;
			if (mp.count(s)) {
				mp.erase(s);
				cout << "Deleted successfully\n";
			} else cout << "Not found\n";
		}
		if (op == 4) {
			cout << mp.size() << endl;
		}
	}
	return 0;
}
