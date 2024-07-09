#include <bits/stdc++.h>
using namespace std;
int main() {
	int n;
	cin >> n;
	int cnt = n % 2 + (n % 3 == 0);
	if (cnt == 2) cout << "1 1 0 0" << endl;
	if (cnt == 1) cout << "0 1 1 0" << endl;
	if (cnt == 0) cout << "0 0 0 1" << endl;
	return 0;
}
