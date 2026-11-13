#include <bits/stdc++.h>
using namespace std;
int days[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
int add(int dt) {
	int y = dt / 10000, m = dt / 100 % 100, d = dt % 100;
	int maxd = days[m];
	if (m == 2 && (y % 4 == 0 && y % 100 != 0 || y % 400 == 0)) maxd = 29;
	d++;
	if (d > maxd) {
		d = 1;
		m++;
	}
	if (m == 13) {
		m = 1;
		y++;
	}
	return y * 10000 + m * 100 + d;
}
int n, day[5005], ty[5005], tim[3], play[3][50005];
string s;
int main() {
	cin >> n;
	for (int i = 1; i <= n; i++) {
		cin >> s >> day[i];
		ty[i] = s == "O" ? 0 : s == "C" ? 1 : 2;
		cin >> s;
		int t = s == "M" ? 0 : s == "A" ? 1 : 2, l = i;
		for (int j = 1; j <= tim[t]; t++) {
			int k = play[t][j];
			if (day[k] == day[l]) {
				if (ty[k] > ty[l] || ty[k] == ty[l] && k > l) {
					swap(play[t][j], l);
				}
				day[l] = add(day[l]);
			} else if (day[k] > day[l]) swap(play[t][j], l);
		}
		tim[t]++;
		play[t][tim[t]] = l;
		cout << i <<endl;
	}
	for (int i = 1; i <= n; i++) cout << day[i] << endl;
	return 0;
}
