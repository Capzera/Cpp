#include <bits/stdc++.h>
using namespace std;
string s[10001];
int n, m;
unordered_map<string, int> mpR, mpC;
vector<int> kmp(vector<int>& a) {
	vector<int> next(a.size() + 1);
	int i = 0, j = next[0] = -1;
	while (i < a.size()) {
		if (j == -1 || a[i] == a[j]) {
			i++, j++;
			next[i] = j;
		} else j = next[j];
	}
	return next;
}
int main() {
	cin >> n >> m;
	vector<int> row, col;
	for (int i = 0, id = 0; i < n; i++) {
		cin >> s[i];
		if (!mpR.count(s[i])) mpR[s[i]] = ++id;
		row.push_back(mpR[s[i]]);
	}
	for (int j = 0, id = 0; j < m; j++) {
		string t;
		for (int i = 0; i < n; i++) {
			t += s[i][j];
		}
		if (!mpC.count(t)) mpC[t] = ++id;
		col.push_back(mpC[t]);
	}
	vector<int> nextR = kmp(row), nextC = kmp(col);
	cout << (n - nextR[n]) * (m - nextC[m]);
	return 0;
}
