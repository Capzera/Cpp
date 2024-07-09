#include <bits/stdc++.h>
using namespace std;
int main() {
	for (int a = 1; a <= 9; a++) {
		for (int c = 0; c <= 9; c++) {
			int num = a * 1000 + a * 100 + c * 10 + c;
			int s = sqrt(num);
			if (s * s == num) cout << num << " ";
		}
	}
	return 0;
}
