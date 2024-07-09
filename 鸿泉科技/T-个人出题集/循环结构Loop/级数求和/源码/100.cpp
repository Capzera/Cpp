#include <bits/stdc++.h>
using namespace std;
int main() {
	int k, n = 0;
	cin >> k;
	for (double sum = 0; sum <= k; n++, sum += 1.0 / n);
	cout << n << endl;
	return 0;
}
