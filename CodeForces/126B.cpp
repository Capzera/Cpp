#include <bits/stdc++.h>
using namespace std;
string s;
int ans;
int main() {
	cin >> s;
	int n = s.size();
	vector<int> next(n + 1);
	int i = 0, j = next[0] = -1;
	while (i < n) {
		if (j == -1 || s[i] == s[j]) {
			i++, j++;
			next[i] = j;
		} else j = next[j];
	}
	int mx = *max_element(next.begin() + 1, next.begin() + n);
	ans = next[n];
	while (ans > mx) ans = next[ans];
	if (ans < 1) return puts("Just a legend"), 0;
	cout << s.substr(0, ans);
	return 0;
}
