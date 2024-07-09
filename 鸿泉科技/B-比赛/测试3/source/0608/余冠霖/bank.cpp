#include <iostream>
using namespace std;

int main() {
	int c = 0, c1 = 0, m;
	cin >> m;
	for (int i = 1; c  < m; i++) {
		c1 += m - c;
		c += i;
	}
	cout << c1;
	return 0;
}
