#include <bits/stdc++.h>
using namespace std;
int h1, h2, h3, h4;
int main() {
	cin >> h1 >> h2 >> h3 >> h4;
	if (abs(h1 - h3) < abs(h1 - h2) || abs(h1 - h3) == abs(h1 - h2) && h3 < h2) swap(h2, h3);
	if (abs(h1 - h4) < abs(h1 - h2) || abs(h1 - h4) == abs(h1 - h2) && h4 < h2) swap(h2, h4);
	cout << h2;
	return 0;
}
