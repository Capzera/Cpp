#include <bits/stdc++.h>
using namespace std;
int x, y, z;
char ch;
int main() {
	cin >> x >> y >> z >> ch;
	if (ch == 'P') x += 12;
	cout << x * 3600 + y * 60 + z;
	return 0;
}
