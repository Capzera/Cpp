#include <bits/stdc++.h>
using namespace std;
int a, b;
int main() {
	cin >> a >> b;
	cout << (b + 10 - a % 10) % 10;
	return 0;
}
