#include <bits/stdc++.h>
using namespace std;
int x, sum;
int main() {
	for (int i = 0; i < 5; i++) {
		cin >> x;
		sum += x;
	}
	if (sum >= 230) return puts("Red"), 0;
	if (sum >= 170) return puts("Orange"), 0;
	if (sum >= 120) return puts("Green"), 0;
	if (sum >= 100) return puts("Blue"), 0;
	return puts("Grey"), 0;
}
