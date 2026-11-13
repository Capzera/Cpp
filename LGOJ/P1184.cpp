#include <bits/stdc++.h>
using namespace std;
string s;
int n, m, ans;
unordered_set<string> mp;
int main() {
	cin >> n >> m;
	cin.ignore();
	while (n--) {
		getline(cin, s);
		mp.insert(s);
	}
	while (m--) {
		getline(cin, s);
		if (mp.count(s)) ans++;
	}
	cout << ans;
	return 0;
}
