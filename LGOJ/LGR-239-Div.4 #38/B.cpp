#include <bits/stdc++.h>
using namespace std;
char x, y;
int z;
int main() {
	cin >> x >> y >> z;
	if (x == 'A' && y == 'N' && z == 2) cout << "A320";
	if (x == 'A' && y == 'W' && z == 2) cout << "A350";
	if (x == 'A' && y == 'W' && z == 4) cout << "A380";
	if (x == 'B' && y == 'N' && z == 2) cout << "B737";
	if (x == 'B' && y == 'W' && z == 4) cout << "B747";
	if (x == 'B' && y == 'W' && z == 2) cout << "B787";
	if (x == 'C' && y == 'N' && z == 2) cout << "C919";
	return 0;
}
