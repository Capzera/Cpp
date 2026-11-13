#include <bits/stdc++.h>
using namespace std;
string s, t1, ans = "*";
int f1, f2 = 1, t2;
int main() {
	getline(cin, s);
	for (int i = 0; i < s.size(); i++) {
		if (s[i] == '(' || s[i] == ')') {
			swap(f1, f2);
			if (s[i] == ')') i++;
			continue;
		}
		if (s[i] == ']') {
			for (int j = 0; j < t2; j++) ans += t1;
			t1 = "", t2 = 0;
		} else if (f1) t1 += s[i];
		else t2 = t2 * 10 + s[i] - '0';
	}
	cout << ans << '*';
	return 0;
}
