#include <bits/stdc++.h>
using namespace std;
long long t, a;
string s1, s2, ch;
int main() {
	cin >> t;
	while (t--) {
		cin >> a >> s1 >> ch >> ch >> s2;
		long long b = a;
		if (s1[0] == 'k') b *= 1000;
		if (s2[0] == 'm' && s2.size() > 1) b *= 1000;
		cout << a << " " << s1 << " = " << b << " " << s2 << endl;
	}
	return 0;
}
