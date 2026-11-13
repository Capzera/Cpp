#include <bits/stdc++.h>
#define int long long
using namespace std;
int t, n, m;
class table {
public:
	int n, m;
	string title;
	string name[11];
	string val[101][11];
	map<string, int> s_n;
	int size;
};
map<string, int> table_se;
string tt, na;
signed main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	cin >> t;
	table a[t];
	for (int xx = 0; xx < t; xx++) {
		
		cin >> tt >> n >> m;
		a[xx].n = n, a[xx].m = m;
		table_se[tt] = xx;
		a[xx].title = tt;
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < m; j++) {
				cin >> a[xx].val[i][j];
				if (i == 0) a[xx].s_n[a[xx].val[0][j]] = j;
			}
		}
	}
	cin >> t;
	cin.ignore();
	while (t--) {
		string opt;
		getline(cin, opt);
		int c1 = opt.find("from");
		int c2 = opt.find("where");
		int c3 = opt.find("=");
		string sa = opt.substr(7, c1 - 8);
		string sb = opt.substr(c1 + 5, c2 - c1 - 6);
		string sc = opt.substr(c2 + 6, c3 - c2 - 6);
		string s_val = opt.substr(c3 + 1);
		int idx = table_se[sb], col;
		table se = a[idx];
		for (int j = 0; j < se.m; j++) {
			if (se.val[0][j] == sc) {
				col = j;
				break;
			}
		}
		vector<int> row;
		for (int i = 0; i < se.n; i++) {
			if (se.val[i][col] == s_val) {
				row.push_back(i);
			}
		}
		string tmp = "";
		vector<string> word;
		for (int i = 0; i < sa.size(); i++) {
			if (sa[i] == ',') {
				word.push_back(tmp);
				tmp = "";
			} else tmp += sa[i];
		}
		word.push_back(tmp);
		vector<int> order;
		for (auto& wo : word) {
			order.push_back(se.s_n[wo]);
		}
		for (int i = 0; i < row.size(); i++) {
			for (int j = 0; j < order.size(); j++) {
				cout << se.val[row[i]][order[j]] << " ";
			}
			cout << endl;
		}
		
	}
	return 0;
}
