#include <bits/stdc++.h>
using namespace std;
int n;
int main() {
	scanf("%d", &n);
	if (n == 1) return puts("-1"), 0;
	printf("%d", n + 1 >> 1);
	return 0;
}
