#include <bits/stdc++.h>
using namespace std;
long long a, b, rem;
string s;
int main() {
	cin >> a >> b >> s;
	for (int i = 0; i < b; i++) {
		char ans = '0';
		if (s[i] == '1') {
			if (!i && a > 9) return puts("-1"), 0;
			if (rem) ans += (a - rem) % a;
			if (!i) ans += a;
			rem = 0;
		} else if (!i || !rem) {
			ans++;
			rem = 1;
		}
		rem = rem * 10 % a;
		cout << ans;
	}
	return 0;
}
