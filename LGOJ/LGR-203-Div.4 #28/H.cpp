#include <bits/stdc++.h>
#define int long long
using namespace std;
int t, n, tarnum, cnt, sum;
signed main() {
	cin >> t;
	vector<vector<vector<int> > > vec; // i课程号，第几门
	for (int l = 0; l < t; l++) {
		cin >> n;
		vector<vector<int> > a(n, vector<int>(10));
		for (auto& x : a) cin >> x[0];
		sort(a.begin(), a.end());
		for (auto& x : a) cin >> x[1];
		sort(a.begin(), a.end(), [&](auto& x, auto& y){
			return x[1] > y[1];
		});
		for (auto& x : a) cin >> x[2];
		for (int i = 1; i < n; i++) {
			if (a[i][1] == a[i - 1][1]) a[i][2] = a[i - 1][2];
		}
		vec.push_back(a);
	}
	cin >> tarnum;
	for (int i = 0; i < t; i++) { // 第i门课
		for (int j = 0; j < vec[i].size(); j++) { // 第j个学生
			if (vec[i][j][0] != tarnum) continue;
			sum += vec[i][j][2];
			cnt++;
		}
	}
	cout << sum / cnt;
	if (sum % cnt) {
		int g = __gcd(sum, cnt);
		cout << "+" << (sum % cnt) / g << "/" << cnt / g;
	}
	return 0;
}
