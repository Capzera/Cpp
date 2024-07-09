#include <iostream>
#include <algorithm>
using namespace std;

bool compare(int  a, int  b) {
	return  a > b;
}

int main() {
	int a, b, c, maX, n = 0;
	cin >> a >> b >> c;
	int Max = max(a, b + b + c) ;
	int m[Max + 1] = {0};
	for (int i = 1; i <= a ; i++) {
		cin >> m[i];
		n += m[i];
	}
	sort(m + 1, m + a + 2, compare);
	int d =  0;
	for (int i = 1; i <= b; i++) {
		d += m[i] - 2 * m[c + i];
		maX = max(d, maX);

	}
	for (int i = 1; i <= c; i++) {
		n -= 2 * m[i];
		;
	}
	cout << maX + n;
	return 0;
}
