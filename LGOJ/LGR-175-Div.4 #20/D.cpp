#include <bits/stdc++.h>
using namespace std;
long long k, f;
bool check(int x) {
	return x % k == 0 && x % 100 % k == 0 && x / 10 % k == 0;
}
int main() {
	cin >> k;
	for (int i = 100; i <= 999; i++) {
		if (check(i)) f = 1, cout << i << endl;
	}
	if (!f) cout << "None!";
	return 0;
}
