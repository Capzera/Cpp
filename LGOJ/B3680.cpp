#include <bits/stdc++.h>
using namespace std;
struct player {
	string name;
	long long val;
};
bool cmp(const player& a, const player& b) {
	return a.val > b.val;
}
long long n, m, l1, l2, k;
char ch;
string str;
int main() {
	cin >> n >> m;
	vector<player> s, h;
	for (int i = 0; i < n + m; i++) {
		cin >> str >> ch >> k;
		if (ch == 'S') s.push_back({str, k});
		else h.push_back({str, k});
	}
	sort(s.begin(), s.end(), cmp), sort(h.begin(), h.end(), cmp);
	cout << h[2].name << endl;
	for (int i = 5; i <= 8; i++) cout << s[i].name << endl;
	return 0;
}
