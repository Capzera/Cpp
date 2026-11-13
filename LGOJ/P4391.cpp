#include <bits/stdc++.h>
using namespace std;
vector<int> init(string p) {
	vector<int> next(p.size() + 1);
	int j = 0, k = next[0] = -1;
	while (j < p.size()) {
		if (k == -1 || p[j] == p[k]) {
			k++, j++;
			next[j] = p[j] == p[k] ? next[k] : k;
		} else k = next[k];
	}
	return next;
}
string s;
int n;
int main() {
	cin >> n >> s;
	vector<int> next = init(s);
	cout << n - next[n];
	return 0;
}
