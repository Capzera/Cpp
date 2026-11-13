#include <bits/stdc++.h>
using namespace std;
struct stu {
	int id, sid, rid, st, du, ed;
	bool can;
} res[100010];
int n, x, tot, vec[100010], ban[100010];
string op;
vector<int> ins[100010], del[100010];
int hour(string s) {
	return ((s[0] - '0') * 10 + (s[1] - '0')) * 60 + (s[3] - '0') * 10 + (s[4] - '0');
}
bool jud(int s1, int e1, int s2, int e2) {
	return s1 < e2 && s2 < e1;
}
void inv(int sid) {
	vec[sid]++;
	if (vec[sid] >= 3) ban[sid] = 1;
	if (vec[sid] >= 2) {
		for (int id : ins[sid]) {
			res[id].can = 1;
			int rid = res[id].rid;
			for (int j = 0; j < del[rid].size(); j++) {
				if (del[rid][j] != id) continue;
				del[rid].erase(del[rid].begin() + j);
				break;
			}
		}
		ins[sid].clear();
	}
}
int main() {
	cin >> n;
	while (n--) {
		cin >> op;
		if (op[0] == 'r') {
			int sid, rid, du;
			string st_str;
			cin >> sid >> rid >> st_str >> du;
			int st = hour(st_str), ed = st + du;
			if (ban[sid]) {
				cout << "FAIL\n";
				continue;
			}
			if (du > 120) {
				inv(sid);
				cout << "FAIL\n";
				continue;
			}
			bool flg = 0;
			for (int id : ins[sid]) {
				stu r = res[id];
				if (jud(st, ed, r.st, r.ed)) {
					flg = 1;
					break;
				}
			}
			if (flg) {
				inv(sid);
				cout << "FAIL\n";
				continue;
			}
			flg = 0;
			for (int id : del[rid]) {
				stu r = res[id];
				if (jud(st, ed, r.st, r.ed)) {
					flg = 1;
					break;
				}
			}
			if (flg) {
				cout << "FAIL\n";
				continue;
			}
			tot++;
			res[tot] = {tot, sid, rid, st, du, ed, 0};
			ins[sid].push_back(tot);
			del[rid].push_back(tot);
			cout << "SUCCESS " << tot << endl;
		} else {
			cin >> x;
			if (x < 1 || x > tot || res[x].can) {
				cout << "FAIL\n";
				continue;
			}
			res[x].can = 1;
			int sid = res[x].sid, rid = res[x].rid;
			for (int j = 0; j < ins[sid].size(); j++) {
				if (ins[sid][j] != x) continue;
				ins[sid].erase(ins[sid].begin() + j);
				break;
			}
			for (int j = 0; j < del[rid].size(); j++) {
				if (del[rid][j] != x) continue;
				del[rid].erase(del[rid].begin() + j);
				break;
			}
			cout << "SUCCESS\n";
		}
	}
	return 0;
}
