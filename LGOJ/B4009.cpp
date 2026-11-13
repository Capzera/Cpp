#include<iostream>
using namespace std;
int x, y, z, w, c;
int main() {
	cin >> x >> y >> z >> w;
	if (!z && !w) return puts("-1"), 0;
	int c = z ? x / z : y / w;
	cout << (x == z * c && y == w * c && c > 0 ? c : -1);
	return 0;
}

