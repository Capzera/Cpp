#include <bits/stdc++.h>
using namespace std;
int main() {
	int a, m, n;
	cin >> a >> m >> n;
	if (a % m == 0 && a % n == 0) {
		cout << "Goal" << endl;
	} else cout << "Bad" << endl;
	return 0;
}
