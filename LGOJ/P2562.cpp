#include <bits/stdc++.h>
using namespace std;
string s, ans;
string fun(string s) {
	string t = s;
	sort(t.begin(), t.end());
	if (t[0] == t.back() && t[0] == '0') return "A";
	if (t[0] == t.back() && t[0] == '1') return "B";
	int n = s.size();
	return "C" + fun(s.substr(0, n / 2)) + fun(s.substr(n / 2));
}
int main() {
	cin >> s;
	cout << fun(s);
	return 0;
}
