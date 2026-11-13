#include <bits/stdc++.h>
using namespace std;
int n = 3;
const char *mp[] = {"A", "AA", "B", "AC", "D", "BC"};
void read(vector<vector<int> > &a) {
	for (auto& i : a)
		for (auto& x : i) 
			cin >> x;
}
void rotate(vector<vector<int> > &a) {
	vector<vector<int> > b = a;
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			b[2 - j][i] = a[i][j];
	a = b;
}
int main() {
	vector<vector<int> > a(n, vector<int>(n)), b = a;
	read(a), read(b);
	if (a == b) return puts("AB"), 0;
	for (int i = 0; i < n; i++) {
		rotate(a);
		if (a == b) return puts(mp[i]), 0;
	}
	rotate(a);
	for (auto& x : a) reverse(x.begin(), x.end());
	if (a == b) return puts("C"), 0;
	for (int i = 0; i < n; i++) {
		rotate(a);
		if (a == b) return puts(mp[i + 3]), 0;
	}
	puts("Poland cannot into space!!!");
	return 0;
}
