#include <bits/stdc++.h>
using namespace std;
string a, b;
int n, m, cnt, now;
int main() {
	cin >> n >> m >> a >> b;
	for (auto& x : b) cnt += x == '#';
	for (auto& x : a) {
		if (x != '#') continue;
		if (now && cnt >= 26 - now) {
			cnt -= 26 - now;
			now = 0;
		}
		x = 'a' + now;
		now = (now + 1) % 26;
	}
	cout << a;
	return 0;
}
