#include <bits/stdc++.h>
using namespace std;
long long n, m, q, loc;
string s, a[11];
bool get(char x) {
	return x == 'a' || x == 'o' || x == 'i' || x == 'u' || x == 'e';
}
int main() {
	cin >> n >> q;
	for (int i = 1; i <= n; i++) cin >> a[i];
	while (q--) {
		int f = 0;
		cin >> s;
		for (int i = 1; i <= n; i++) {
			if (s.find(a[i]) == -1) continue;
			m = a[i].size();
			loc = s.find(a[i]);
			while (loc != -1) {
				if (loc && !get(s[loc - 1]) && (get(s[loc]) || s[loc - 1] != 'n')) {
					loc = s.find(a[i], loc + 1);
					continue;
				}
				if (s[loc + m - 1] != 'n' || loc + m >= s.size() || !get(s[loc + m])) {
					f++;
					break;
				} 
				loc = s.find(a[i], loc + 1);
			}
		}
		cout << (f == 1 ? "Yes" : "No") << ", Commander\n";
	}
	return 0;
}
