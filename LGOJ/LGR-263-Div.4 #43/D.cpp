#include <bits/stdc++.h>
using namespace std;
int n, m, k, l, r, mx;
bool cmp(pair<int, int> lhs, pair<int, int> rhs) {
	if (lhs.second != rhs.second) return lhs.second < rhs.second;
	return lhs.first < rhs.first;
}
int main() {
	cin >> n >> m;
	for (int i = 0; i < n; i++) {
		vector<pair<int, int>> vec;
		cin >> k;
		for (int j = 0; j < k; j++) {
			cin >> l >> r;
			vec.push_back({l, r});
		}
		sort(vec.begin(), vec.end(), cmp);
		int flag = 0;
		for (int j = 0; j < k; j++) {
			if (vec[j].first > mx) {
				mx = vec[j].second;
				flag = 1;
				break;
			}
		}
		if (!flag) return puts("-1"), 0;
	}
	cout << (mx <= m ? mx : -1);
	return 0;
}
