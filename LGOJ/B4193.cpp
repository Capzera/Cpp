#include <bits/stdc++.h>
using namespace std;
string s;
int n, a, b, tot, ans;
int main() {
	cin >> n >> a >> b >> s;
	tot = a + b;
	s += "*";
	vector<int> num;
	for (int i = 0, cnt = 0; i < s.size(); i++) {
		if (s[i] == '*') {
			if (cnt) num.push_back(cnt);
			cnt = 0;
		} else cnt++;
	}
	for (int i = 0; i < num.size(); i++) {
		int lot = num[i] + 1 >> 1, les = lot - (num[i] & 1);
		if (a > b) {
			a -= min(a, lot);
			b -= min(b, les);
		} else {
			a -= min(a, les);
			b -= min(b, lot);
		}
	}
	cout << tot - a - b;
	return 0;
}
