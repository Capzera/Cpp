#include <bits/stdc++.h>
using namespace std;
long long f() {
	long long wl, wd, rl, rd;
	cin >> wl >> wd >> rl >> rd;
	if (!wl) wl = f();
	if (!rl) rl = f();
	if (wl < 0 || rl < 0) return -1;
	if (wl * wd == rl * rd) return wl + rl;
	return -1;
}
int main() {
	cout << (f() >= 0);
	return 0;
}
