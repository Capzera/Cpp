#include <bits/stdc++.h>
using namespace std;
string s;
int mirror(string s) {
	int n = s.size();
	if (n & 1) return n;
	string s1 = s.substr(0, n / 2);
	string s2 = s.substr(n / 2);
	reverse(s2.begin(), s2.end());
	if (s1 == s2) return mirror(s1);
	return n;
}
int main() {
	cin >> s;
	cout << mirror(s);
	return 0;
}
