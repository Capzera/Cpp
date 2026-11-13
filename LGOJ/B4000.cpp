#include <bits/stdc++.h>
using namespace std;
int h, m, s, diff, second;
int main() {
	cin >> h >> m >> s >> diff;
	second = h * 3600 + m * 60 + s + diff;
	printf("%d %d %d", second / 3600, second % 3600 / 60, second % 60);
	return 0;
}
