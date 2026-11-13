#include <bits/stdc++.h>
using namespace std;
string s;
long long a, b, c, f, x;
int main() {
	cin >> s;
	for (int i = 0; i < s.size(); i++) {
		if (s[i] == ';') {
			if (f == 1) a = x;
			if (f == 2) b = x;
			if (f == 3) c = x;
			f = 0;
		}
		if (s[i] == 'a' && !f) f = 1;
		if (s[i] == 'b' && !f) f = 2;
		if (s[i] == 'c' && !f) f = 3;
		if (s[i + 1] == ';') {
			if (s[i] == 'a') x = a;
			if (s[i] == 'b') x = b;
			if (s[i] == 'c') x = c;
			if (isdigit(s[i])) x = s[i] - '0';
		}
	}
	cout << a << " " << b << " " << c;
	return 0;
}
