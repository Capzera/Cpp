#include <bits/stdc++.h>
using namespace std;
int main() {
	int n;
	cin >> n;
	vector<vector<int>> a(n, vector<int>(2));
	for (int i = 0; i < n; i++) {
		cin >> a[i][0] >> a[i][1];
	}
	sort(a.begin(), a.end());
	vector<int> v;
	v.push_back(a[0][1]);
	for (int i = 1; i < n; i++) {
		bool flag = 1;
		for (int j = 0; j < v.size(); j++) {
			if (a[i][0] < v[j]) continue;
			flag = 0;
			v[j] = a[i][1];
			break;
		}
		if (flag) {
			v.push_back(a[i][1]);
		}
	}
	cout << v.size() << endl;
	return 0;
}
