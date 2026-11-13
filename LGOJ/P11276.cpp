#include <bits/stdc++.h>
using namespace std;
string p;
int n;
int main() {
	cin >> p;
	n = p.size();
	vector<int> next(n + 1);
	int j = 0, k = next[0] = -1;
	while (j < n) {
		if (k == -1 || p[j] == p[k]) {
			k++, j++;
			next[j] = p[j] == p[k] ? next[k] : k;
		} else k = next[k];
	}
	string ans = p + p.substr(next[n]);
	cout << ans;
	return 0;
}
