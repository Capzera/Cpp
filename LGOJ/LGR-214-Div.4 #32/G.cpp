#include <bits/stdc++.h>
using namespace std;
vector<string> a;
string mp[] = {"0000", "0001", "0010", "0011", "0100", "0101", "0110", "0111", "1000", "1001", "1010", "1011", "1100", "1101", "1110", "1111"};
string s, t;
int main() {
	cin >> s;
	s += ",";
	for (int i = 0; i < s.size(); i++) {
		if (s[i] == ':' || s[i] == ',') {
			a.push_back(t);
			t= "";
		} else t += s[i];
	}
	if (a.size() < 8) {
		int i = 0;
		for (; i < a.size(); i++) {
			if (a[i] == "") {
				a[i] = "0000";
				break;
			}
		}
		while (a.size() < 8) {
			a.insert(a.begin() + i, "0000");
		}
	}
	for (auto& x : a) {
		while (x.size() < 4) x = "0" + x;
		for (auto& ch : x) {
			int i = ch - 48 - 7 * (ch > '9');
			cout << mp[i];
		}
	}
	return 0;
}
