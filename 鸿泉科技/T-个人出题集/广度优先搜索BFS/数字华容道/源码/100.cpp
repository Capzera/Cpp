#include <bits/stdc++.h>
using namespace std;
int main() {
	string str, target = "123456789ABCDEF*";
	vector<int> move = {-1, 1, -4, 4};
	unordered_map<string, int> mp;
	queue<pair<string, int> > q;
	int x, x1, x2, depth = 0;
	for (int i = 0; i < 16; i++) {
		cin >> x;
		if (x < 10) str += to_string(x);
		else str.push_back(char(x + 55));
	}
	cout << str << endl;
	/*
	q.push();
	while (q.size()) {
		int size = q.size();
		while (size--) {
			auto [t, step] = q.front(); q.pop();
			if (um.count(t)) continue;
			mp[t] = step;
			int space = t.find('*'), a = t.find('A'), b = t.find('B');
			if (a == x2 && b == x1) {
				cout << depth << endl;
				return 0;
			}
			for (int d = 0; d < 4; d++) {
				if (d == 0 && space % 3 == 0) continue;
				if (d == 1 && space % 3 == 2) continue;
				if (d == 2 && space < 3) continue;
				if (d == 3 && space > 2) continue;
				if (space + move[d] < 0 || space + move[d] > 5) continue;
				swap(t[space], t[space + move[d]]);
				q.push(t);
				swap(t[space], t[space + move[d]]);
			}
		}
		depth++;
	}*/
	return 0;
}
/*

*/
