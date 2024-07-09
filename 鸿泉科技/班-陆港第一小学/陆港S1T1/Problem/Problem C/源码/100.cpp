#include <bits/stdc++.h>
using namespace std;
int main() {
	int t, m, s, v;
	cin >> t >> m >> s >> v;
	int time = t * 60 - m - s / v;
	if (s % v) time--;
	cout << time / 60 << " " << time % 60;
	return 0;
}
