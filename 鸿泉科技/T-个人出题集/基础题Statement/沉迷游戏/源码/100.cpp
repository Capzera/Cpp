#include <bits/stdc++.h>
using namespace std;
int main() {
	int a, b, c, d;
	cin >> a;
	cin >> b;
	cin >> c;
	cin >> d;
	int diff = c * 60 + d - a * 60 - b;
	cout << diff / 60 << " " << diff % 60 << endl;
	return 0;
}
