#include <bits/stdc++.h>
using namespace std;
string s;
vector<vector<string> > word;
unordered_map<char, int> mp;
int mx, t, cnt;
int main() {
	while (cin >> s, s != "ENDOFINPUT") {
		cin.ignore();
		getline(cin, s);
		stringstream buf(s);
		vector<string> line;
		while (buf >> s) {
			for (auto& ch : s) {
				ch = toupper(ch);
				if (isalpha(ch)) mp[ch]++;
			}
			line.push_back(s);
		}
		word.push_back(line);
		cin >> s;
	}
	for (auto&& [x, y] : mp) {
		if (y <= mx) continue;
		t = (x - 'A' + 22) % 26, mx = y;
	}
	for (auto& line : word)
		for (auto& x : line)
			for (auto& ch : x)
				if (isalpha(ch)) ch = (ch - 'A' - t + 26) % 26 + 'A';
	for (auto& line : word) {
		for (auto& x : line) cout << x << " ";
		cout << endl;
	}
	return 0;
}
