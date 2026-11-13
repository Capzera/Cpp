#include <bits/stdc++.h>
using namespace std;
string fun() {
	string ans = "";
	char ch;
	int n;
	while (cin >> ch) {
		if (ch == '\n') break;
		if (ch == '[') {
			cin >> n;
			string s = fun();
			while (n--) ans += s;
		}
		else if (ch == ']') return ans;
		else ans += ch;
	}
	return ans;
}
int main() {
	cout << fun();
	return 0;
}
