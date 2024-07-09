#include <bits/stdc++.h>
using namespace std;
int main() {
	int n;
	cin >> n;
	int day = ((n + 8) / 24) % 7;
	int st = day < 2 ? 7 : 17;
	n = (n + 9) % 24;
	cout << (n >= st && n <= 21 ? "yes" : "no") << endl;
	cout << n << " " << n + 1 << endl;
	return 0;
}
