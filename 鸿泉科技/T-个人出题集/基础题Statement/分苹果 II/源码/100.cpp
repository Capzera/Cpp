#include <bits/stdc++.h>
using namespace std;
int main() {
	int n, k, t;
	cin >> n;
	cin >> k;
	cin >> t;
	cout << (((n * k - t) * k - t) * k - t) << endl;
	return 0;
}
