#include <bits/stdc++.h>
using namespace std;
int main() {
	long long n, pro = 1, sum = 0;
	while (n) {
		pro *= n % 10;
		sum += n % 10;
		n /= 10;
	}
	cout << pro - sum;
	return 0;
}
