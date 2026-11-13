#include <bits/stdc++.h>
using namespace std;
string rev(string n) {
	string ans = "";
	if (n == "0") return "0";
	int flag = 0;
	if (n[0] == '-') {
		flag = 1;
		n = n.substr(1);
	}
	while (n.size() > 1 && n.back() == '0') n.pop_back();
	if (flag) n += "-";
	reverse(n.begin(), n.end());
	while (n.size() > 1 && n.back() == '0') n.pop_back();
	return n;
}  
int main() {
	string s, ans;
	cin >> s;
	int n = s.size(), split = -1;
	for (int i = 0; i < n; i++) {
		if (s[i] == '.' || s[i] == '/') {
			split = i;
			break;
		}
	}
	if (s.back() == '%') {
		ans = rev(s.substr(0, n - 1)) + "%";
	} else if (split == -1) {
		ans = rev(s);
	} else {
		ans = rev(s.substr(0, split)) + s[split] + rev(s.substr(split + 1));
	}
	cout << ans;
	return 0;
}
