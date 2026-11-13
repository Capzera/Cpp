#include <bits/stdc++.h>
using namespace std;
long long n;
string s, ori, t;
int main() {
	cin >> n >> s >> ori;
	n <<= 1;
	vector<string> card, shuffle;
	s += ",";
	for (auto& ch : s) {
		if (ch == ',') {
			card.push_back(t);
			t = "";
		} else t += ch;
	}
	for (int i = 0, l = 0, r = n / 2; i < n; i++) {
		if (ori[i] == 'L') shuffle.push_back(card[l++]);
		else shuffle.push_back(card[r++]);
	}
	for (int i = n - 1; i >= 0; i -= 2) cout << shuffle[i] << endl;
	return 0;
}
