#include <bits/stdc++.h>
using namespace std;
int main() {
	int n;
	cin >> n;
	vector<vector<int> > a(n + 1, vector<int>(5));
	for (int i = 1; i <= n; i++) {
		a[i][0] = i;
		cin >> a[i][1] >> a[i][2] >> a[i][3];
		a[i][4] = a[i][1] + a[i][2] + a[i][3];
	}
	sort(a.begin() + 1, a.end(), [&](auto& a, auto& b){
		return a[4] != b[4] && a[4] > b[4] || (a[4] == b[4] && a[1] != b[1] && a[1] > b[1]) || (a[4] == b[4] && a[1] == b[1] && a[0] < b[0]);
	});
	for (int i = 1; i <= 5; i++) {
		cout << a[i][0] << " " << a[i][1] + a[i][2] + a[i][3] << endl;
	}
	return 0;
}
