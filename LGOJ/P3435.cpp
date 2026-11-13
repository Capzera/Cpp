#include <bits/stdc++.h>
using namespace std;
string s;
long long n, ans;
vector<int> kmp(string s) {
	vector<int> next(n + 1);
	int j = 0, k = next[0] = -1;
	while (j < n) {
		if (k == -1 || s[j] == s[k]) {
			j++, k++;
			next[j] = k;
		} else k = next[k];
	}
	return next;
}
int main() {
	cin >> n >> s;
	vector<int> next = kmp(s);
	for (int i = 1; i <= n; i++) {
		if (!next[i]) continue;
		int j = next[i];
		while (next[j]) j = next[j]; // 找到 minimum 的 border，因为border 的 border 还是 border
		next[i] = j;
		ans += i - j;
	}
	cout << ans;
	return 0;
}

