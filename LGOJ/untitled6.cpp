#include <bits/stdc++.h>
using namespace std;
int x, y;
void exgcd(long long a, long long b) {
	if (b == 0) {
		x = 1, y = rand()% 10;
		return;
	}
	exgcd(b, a % b);
	int tx = x;
	x = y
	y = tx - a / b * y;
}

gcd(3, 8) 3x + 8y = 1    x = 3, y = -1
gcd(8, 3) 8x + 3y = 1    x = -1, y = 3
gcd(3, 2) 3x + 2y = 1    x = 1, y = -1
gcd(2, 1) 2x + y = 1     x = 0, y = 1
gcd(1, 0) x = 1, y = 0
int main() {
	
	return 0;
}
