#include <bits/stdc++.h>
using namespace std;
string s, t;
int n;
vector<int> kmp(string s) {
	vector<int> next(s.size() + 1);
	int j = 0, k = next[0] = -1;
	while (j < s.size()) {
		if (k == -1 || s[j] == s[k]) {
			j++, k++;
			next[j] = k;
		} else k = next[k];
	}
	return next;
}
int main() {
	cin >> n >> s;
	t = s;
	reverse(t.begin(), t.end());
	vector<int> next = kmp(t + "#" + s);
	cout << n - next[n << 1 | 1];
	return 0;
}
