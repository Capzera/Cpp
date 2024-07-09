#include <bits/stdc++.h>
using namespace std;
int main() {
	int n, m;
	cin >> n;
	cin >> m;
	vector<vector<int> > a(n, vector<int>(m));
	for (auto& i : a) {
		for (auto& j : i) cin >> j;
	}
	for (auto& i : a) {
		reverse(i.begin(), i.end());
	}
	for (auto& i : a) {
		for (auto& j : i) cout << j << " ";
		cout << endl;
	}
	return 0;
}
