#include <bits/stdc++.h>
using namespace std;
int a, b, c;
int main() {
	cin >> a >> b >> c;
	if (a > b && c < a) {
		cout << "LeftToRight";
		return 0;
	}
	if (a < b && c < b) {
		cout << "RightToLeft";
		return 0;
	}
	cout << "None";
	return 0;
}
