#include <bits/stdc++.h>
using namespace std;
int t, n, x;
unordered_map<int, int> vec;
vector<unordered_map<int, int>> rec;
long long ti[100010];
int main() {
	cin >> t;
	for (int i = 0, j = 0; i < t; i++) {
		cin >> ti[i] >> n;
		unordered_map<int, int> mp;
		while (n--) {
			cin >> x;
			mp[x]++, vec[x]++;
		}
		while ((j < i) && (ti[i] - ti[j] >= 86400)) {
			for (auto&& [x, y] : rec[j]) {
				vec[x] -= y;
				if (!vec[x]) vec.erase(x);
			}
			j++;
		}
		rec.push_back(mp);
		cout << vec.size() << endl;
	}
	return 0;
}
