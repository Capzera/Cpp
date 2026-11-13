#include <bits/stdc++.h>
using namespace std;
string s;
long long n, a, b, c;
int main() {
	cin >> s;
	n = s.size();
	int loc = s.find('+');
	if (loc == -1) loc = s.find('-');
	if (loc == -1) loc = s.find('*');
	if (loc == -1) loc = s.find('/');
	char op = loc != -1 ? s[loc] : ' ';
	if (s.back() == '?') {
		a = stoll(s.substr(0, loc));
		b = stoll(s.substr(loc + 1, n - loc - 3));
		if (op == '+') cout << a + b;
		if (op == '-') cout << a - b;
		if (op == '*') cout << a * b;
		if (op == '/') cout << a / b;
		return 0;
	}
	c = stoll(s.substr(s.find('=') + 1));
	int que = s.find('?');
	if (loc == -1) {
		a = stoll(s.substr(0, loc));
		b = stoll(s.substr(que + 1, s.find('=') - 1 - que));
		if (a + b == c) return puts("+"), 0;
		if (a - b == c) return puts("-"), 0;
		if (a * b == c) return puts("*"), 0;
		return puts("/"), 0;
	}
	if (que == 0) {
		b = stoll(s.substr(loc + 1, s.find('=') - 1 - loc));
		if (op == '+') cout << c - b;
		if (op == '-') cout << c + b;
		if (op == '*') cout << c / b;
		if (op == '/') cout << c * b;
		return 0;
	}
	a = stoll(s.substr(0, loc));
	if (op == '+') cout << c - a;
	if (op == '-') cout << a - c;
	if (op == '*') cout << c / a;
	if (op == '/') cout << a / c;
	return 0;
}
