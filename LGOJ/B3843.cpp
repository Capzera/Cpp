#include <bits/stdc++.h>
using namespace std;
string s, tmp;
bool check(string s) {
	int f1 = 0, f2 = 0, f3 = 0, f4 = 0;
	if (s.size() < 6 || s.size() > 12) return 0;
	for (auto& ch : s) {
		if (islower(ch)) f1 = 1;
		else if (isupper(ch)) f2 = 1;
		else if (isdigit(ch)) f3 = 1;
		else if (ch == '!' || ch == '@' || ch == '#' || ch == '$') f4 = 1;
		else return 0;
	}
	return f4 && (f1 + f2 + f3 > 1);
}
int main() {
	cin >> s;
	s += ',';
	for (auto& ch : s) {
		if (ch == ',') {
			if (check(tmp)) cout << tmp << endl;
			tmp = "";
		} else tmp += ch;
	}
	return 0;
}

