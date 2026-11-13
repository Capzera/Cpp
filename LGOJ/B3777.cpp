#include <bits/stdc++.h>
using namespace std;
int n;
int main() {
	cin >> n;
	if (n == 2) return puts("8 0 0"), 0;
	printf("%d %d %d", 8, 12 * n - 24, 6 * (n - 2) * (n - 2));
	return 0;
}
