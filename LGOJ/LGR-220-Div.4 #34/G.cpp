#include <bits/stdc++.h>
using namespace std;
string s;
int main() {
	cin >> s;
	for (int i = 0, cnt = 0, sum = 0; i < s.size(); i++) {
		if (isdigit(s[i]) && !isdigit(s[i + 1])) sum += s[i] - '0', cnt++;
		if (s[i] == '.' || s[i] == '!' || s[i] == '?') {
			cout << cnt << " " << (sum & 1 ? "Odd\n" : "Even\n");
			cnt = sum = 0;
		}
	}
	return 0;
}
