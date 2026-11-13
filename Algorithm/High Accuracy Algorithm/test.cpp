#include <bits/stdc++.h>
using namespace std;
int n, T;
string s;
double ans;
int main() {
	cin >> n >> T;
	while (cin >> s) {
		double stk = 1, beat = 1;
		n = s.size();
		for (int i = 0; i < n; i++) {
			if (s[i] == '(') stk /= 2;
			else if (s[i] == ')') stk *= 2;
			else if (s[i] == '.') {
				ans += beat * 0.5;
			} else if (s[i] == '-') {
				beat += stk;
				ans += stk;
			} else {
				beat = stk;
				ans += beat;
			}
		}
	}
	cout << (int)ans * 60 / T;
	return 0;
}
