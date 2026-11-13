#include <bits/stdc++.h>
using namespace std;
int n;
char a, b, c;
void hanoi(int n, char a, char b, char c) {
	if (n == 1) {
		printf("%c->%d->%c\n", a, 1, c);
		return;
	}
	hanoi(n - 1, a, c, b);
	printf("%c->%d->%c\n", a, n, c);
	hanoi(n - 1, b, a, c);
}
int main() {
	cin >> n >> a >> b >> c;
	hanoi(n, a, c, b);
	return 0;
}
