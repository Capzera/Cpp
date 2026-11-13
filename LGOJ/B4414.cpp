#include <bits/stdc++.h>
using namespace std;
const int st[] = {-1, 3, 6, 6, 2, 4, 7, 2, 5, 1, 3, 6, 1};
const int mon[] = {-1, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
int m;
int main() {
	cin >> m;
	cout << "MON TUE WED THU FRI SAT SUN\n";
	for (int x = 1; x < st[m]; x++) cout << "    ";
	for (int i = 1, j = st[m]; i <= mon[m]; i++, j++) {
		if (j == 8) cout << '\n', j = 1;
		printf("%3d ", i);
	}
	return 0;
}


