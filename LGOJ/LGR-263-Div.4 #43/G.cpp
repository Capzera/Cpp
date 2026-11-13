#include <bits/stdc++.h>
using namespace std;
int t, n;
string s, row;
map<char, int> mp;
int get(string s) {
	if (isdigit(s[0])) s = "+" + s;
	s += "+";
	int ans = 0;
	char ch = 0;
	for (int i = 0, k = 1, num = 0; i < s.size(); i++) {
		if (!isdigit(s[i])) {
			if (!isalpha(s[i])) {
				ans += (ch ? mp[ch] : num) * k;
				k = s[i] == '+' ? 1 : -1;
			} else ch = s[i];
			num = 0;
		} else {
			ch = 0;
			num = num * 10 + s[i] - '0';
		}
	}
	return ans;
}
int solve() {
	cin >> n >> s;
	mp.clear();
	string lt = s.substr(0, s.find('=')), rt = s.substr(s.find('=') + 1);
	for (int i = 1; i <= n; i++) {
		cin >> row;
		mp[row[0]] = stoi(row.substr(2));
	}
	return get(lt) == get(rt);
}
int main() {
	cin >> t;
	while (t--) cout << (solve() ? "Yes\n" : "No\n");
	return 0;
}
