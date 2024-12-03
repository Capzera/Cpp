#include <bits/stdc++.h>
using namespace std;
int n;
struct grey {
	int val, cnt;
	bool operator < (const struct grey& rhs) {
		if (cnt != rhs.cnt) return cnt > rhs.cnt;
		return val < rhs.val;
	}
} mp[256];
int get(char x) {
	return x < 'A' ? x - '0' : x - 'A' + 10;
}
char anti_get(int x) {
	return x > 9 ? x - 10 + 'A' : x + '0';
}
int main() {
	cin >> n;
	for (int i = 0; i < 256; i++) mp[i].val = i;
	vector<string> a(n);
	for (auto& x : a) {
		cin >> x;
		for (int i = 0; i < x.size(); i += 2) {
			mp[get(x[i]) * 16 + get(x[i + 1])].cnt++;
		}
	}
	sort(mp, mp + 256);
	for (int i = 0; i < 16; i++) {
		cout << anti_get(mp[i].val / 16) << anti_get(mp[i].val % 16);
		
	}
	cout << endl;
	for (auto& x : a) {
		for (int i = 0; i < x.size(); i += 2) {
			int now = get(x[i]) * 16 + get(x[i + 1]), mn = 256, loc = -1;
			for (int j = 0; j < 16; j++) {
				if (abs(now - mp[j].val) < mn) {
					mn = abs(now - mp[j].val);
					loc = j;
				}
			}
			cout << anti_get(loc);
		}
		cout << endl;
	}
	return 0;
}
