#include <bits/stdc++.h>
using namespace std;
void init(string p, vector<int>& next) {
	int j = 0, k = next[0] = -1;
	while (j < p.size()) {
		if (k == -1 || p[j] == p[k]) {
			k++, j++;
			//next[j] == p[j] == p[k] ? next[k] : k;
			next[j] = k;
		} else k = next[k];
	}
}
void KMP(string s, string p) {
	int i = 0, j = 0;
	int slen = s.size(), plen = p.size();
	vector<int> next(plen + 1);
	init(p, next);
	while (i < slen) {
		if (j == plen) {
			cout << i - j + 1 << '\n';
			j = next[plen];
		} else if (j == -1 || s[i] == p[j]) {
			i++, j++;
		} else j = next[j];
	}
	if (j == plen) cout << i - j + 1 << '\n';
	for (int i = 1; i <= plen; i++) {
		cout << next[i] << " ";
	}
}
string src, pat;
int main() {
	cin >> src >> pat;
	KMP(src, pat);
	return 0;
}
