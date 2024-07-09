#include <bits/stdc++.h>
using namespace std;
int main() {
	int a, b, c;
	cin >> a;
	cin >> b;
	cin >> c;
	cout << abs(c - max(a, b)) << endl;
	return 0;
}
