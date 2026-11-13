#include <bits/stdc++.h>
using namespace std;
int h1, m1, h2, m2;
int main() {
	cin >> h1 >> m1 >> h2 >> m2;
	cout << h2 * 60 + m2 - h1 * 60 - m1;
	return 0;
}
