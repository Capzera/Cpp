#include <bits/stdc++.h>
using namespace std;
int main() {
	int n, m, cnt = 1;
	cin >> n;
	cin >> m;
	vector<vector<int> > grid(n, vector<int>(m));
	for (auto& i : grid)
		for (auto& j : i) j = cnt++;
		
	for (auto& i : grid) {
		for (auto& j : i) cout << j << " ";
		cout << endl;
	}
	return 0;
}
