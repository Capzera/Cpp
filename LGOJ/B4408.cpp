#include <bits/stdc++.h>
using namespace std;
int T, h1, m1, h2, m2;
string op, name, docName;

struct info {
	int appTime, regTime, trtTime;
	string patName, docName;
	bool isLate;
	string treat() {
		string name = patName;
		for (int i = 1; i < name.size() - 1; i++) name[i] = '*';
		return name;
	}
	bool operator <(const struct info& rhs) const {
		if (isLate != rhs.isLate) return isLate < rhs.isLate;
		if (!isLate) {
			if (trtTime != rhs.trtTime) return trtTime < rhs.trtTime;
			return appTime < rhs.appTime;
		}
		return regTime < rhs.regTime;
	}
};

vector<info> appInfo, regInfo;

void solve() {
	cin >> h1 >> m1 >> op >> name;
	if (op == "appointment") {
		cin >> docName >> h2 >> m2;
		appInfo.push_back({h1 * 60 + m1, 0, h2 * 60 + m2, name, docName, 0});
	}
	if (op == "register") {
		int loc = -1;
		for (int i = 0; i < appInfo.size(); i++) {
			if (appInfo[i].patName != name) continue;
			if (h1 * 60 + m1 < appInfo[i].trtTime - 60) break;
			loc = i;
		}
		if (loc == -1) {
			puts("Fail");
			return;
		}
		puts("Success");
		regInfo.push_back(appInfo[loc]);
		regInfo.back().regTime = h1 * 60 + m1;
		appInfo.erase(appInfo.begin() + loc);
		sort(regInfo.begin(), regInfo.end());
	}
	if (op == "query") {
		for (int i = 0; i < regInfo.size(); i++) {
			if (regInfo[i].docName != name) continue;
			cout << regInfo[i].treat() << '\n';
			regInfo.erase(regInfo.begin() + i);
			return;
		}
		puts("No patient");
	}
}

int main() {
	cin >> T;
	while (T--) solve();
	return 0;
}
