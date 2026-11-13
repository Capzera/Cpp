#include <bits/stdc++.h>
using namespace std;
int x, n;
int main() {
	cin >> x >> n;
	cout << (x - 1 + n) % 7 + 1;
	return 0;
}

