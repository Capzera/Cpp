#include <bits/stdc++.h>
using namespace std;
int n, m, x;
string s;
map<string, int> mp;
set<string> ans;
int main() {
	cin >> n >> m;
	for (int i = 0; i < m; i++) {
		cin >> x;
		while (x--) {
			cin >> s;
			mp[s] = i; // 将联盟合并为行号
		}
	}
	while (n--) {
		cin >> s;
		string com = s.substr(0, 2);
		string key = (mp.count(com) ? to_string(mp[com]) : com) + s.substr(7); // 二次编码，忽略联盟公司
		ans.insert(key);
	}
	cout << ans.size() << endl; // 统计二次编码的个数
	return 0;
}
