#include <bits/stdc++.h>
using namespace std;
long long p1, p2, p3, n;
string s, ans;
int get(char x) {
	if (isdigit(x)) return 1;
	if (isalpha(x)) return 2;
	return 0;
}
int main() {
	cin >> p1 >> p2 >> p3 >> s;
	n = s.size();
	for (int i = 0; i < n; i++) {
		if (s[i] == '-' && (!i || i == n - 1)) {
			ans += s[i];
			continue;
		}
		if (s[i] != '-' || get(s[i - 1]) != get(s[i + 1]) || s[i - 1] >= s[i + 1]) {
			ans += s[i];
			continue;
		}
		if (s[i - 1] + 1 == s[i + 1]) { // 只跳过-
			ans += s[++i];
			continue;
		}
		string t;
		for (char st = s[i - 1] + 1; st < s[i + 1]; st++) {
			char cur = p1 == 3 ? '*' : st;
			if (p1 == 2 && isalpha(cur)) cur -= 32; 
			for (int i = 0; i < p2; i++) t += cur; 
		}
		if (p3 == 2) reverse(t.begin(), t.end());
		ans += t + s[++i];
	}
	cout << ans;
	return 0;
}
