#include <bits/stdc++.h>
using namespace std;
int main() {
	int a, b, c;
	cin >> a;
	cin >> b;
	cin >> c;
	cout << max(a, max(b, c)) << endl;
	return 0;
}
