#include <bits/stdc++.h>
using namespace std;
string op, s;
map<string, int> mp;
bool check(string s) {
	int c1 = 0, c2 = 0;
	for (auto& c : s) {
		if (c == '.') c1++;
		else if (c == ':') c2++;
		else if (!isdigit(c)) return 0;
	}
	if (c1 != 3 || c2 != 1) return 0;
	if (s.rfind('.') > s.find(':')) return 0;
	vector<string> split;
	string t;
	for (auto& c : s) {
		if (c == '.' || c == ':') {
			split.push_back(t);
			t = "";
		} else t += c;
	}
	split.push_back(t);
	if (split.size() != 5) return 0;
	for (int i = 0; i < 5; i++) {
		t = split[i];
		int len = t.size();
		if (len > 5 || !len || (len > 1 && t[0] == '0')) return 0;
		if (i < 4 && stoi(t) > 255 || i == 4 && stoi(t) > 65536) return 0;
	}
	return 1;
}
void solve(int index) {
	cin >> op >> s;
	if (!check(s)) {
		puts("ERR");
		return;
	}
	if (op == "Server") {
		if (mp.count(s)) {
			puts("FAIL");
			return;
		}
		mp[s] = index;
		puts("OK");
	} else {
		if (!mp.count(s)) {
			puts("FAIL");
			return;
		}
		cout << mp[s] << '\n';
	}
}

int main() {
	int T = 1;
	cin >> T;
	for (int i = 1; i <= T; i++) {
		solve(i);
	}
	return 0;
}
