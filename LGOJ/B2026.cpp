#include <bits/stdc++.h>
using namespace std;
double a, b;
int main() {
	cin >> a >> b;
	int k = a / b;
	cout << a - b * k;
	return 0;
}
