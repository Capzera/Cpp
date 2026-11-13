#include <bits/stdc++.h>
using namespace std;
int c[4], m[4];
void move(int x, int y) {
	int diff = min(m[x], c[y] - m[y]);
	m[x] -= diff;
	m[y] += diff;
}
int main() {
	for (int i = 1; i <= 3; i++) cin >> c[i] >> m[i];
	for (int i = 0; i < 100; i++) {
		if (i % 3 == 0) move(1, 2);
		if (i % 3 == 1) move(2, 3);
		if (i % 3 == 2) move(3, 1);
	}
	for (int i = 1; i <= 3; i++) cout << m[i] << '\n';
	return 0;
}
