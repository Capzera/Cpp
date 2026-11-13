#include <bits/stdc++.h>
using namespace std;
int x, sum;
int main() {
	for (int i = 0; i < 16; i++) {
		cin >> x;
		sum += x;
	}
	cout << 40 - sum;
	return 0;
}
