#include <bits/stdc++.h>
using namespace std;
int main() {
	int a, b, c;
	cin >> a >> b >> c;
	if (a + b > c && a + c > b && b + c > a) {
		cout << "It is a triangle" << endl;
	} else cout << "It isn't a triangle" << endl;
	return 0;
}
