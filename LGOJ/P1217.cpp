#include <bits/stdc++.h>
using namespace std;
long long m, n;
vector<int> prime = {2, 3, 5, 7};
bool check(long long x) {
	if (x == 1) return 0;
	if (x == 2 || x == 3) return 1;
	if (x % 6 != 1 && x % 6 != 5) return 0;
	long long n = sqrt(x);
	for (int i = 5; i <= n; i += 6) {
		if (x % i == 0 || x % (i + 2) == 0) return 0;
	}
	return 1;
}
void op(int &x) {
	if (check(x)) prime.emplace_back(x);
}
void rt() {
	sort(prime.begin(), prime.end());
	for (auto& x : prime) {
		if (x < m) continue;
		if (x > n) return;
		cout << x << endl;
	}
	exit(0);
}
int main() {
	cin >> m >> n;
	if (m <= 100) {
		for (int i = 1; i <= 9; i += 2) {
			int num = i * 10 + i;
			op(num);
		}
	}
	if (n >= 100 && m <= 10000) {
		for (int a = 1; a <= 9; a += 2) {
			for (int b = 0; b <= 9; b++) {
				int num = a * 100 + b * 10 + a;
				op(num);
				num = a * 1000 + b * 110 + a;
				op(num);
			}
		}
	}
	if (n >= 1e4 && m <= 100000) {
		for (int a = 1; a <= 9; a += 2) {
			for (int b = 0; b <= 9; b++) {
				for (int c = 0; c <= 9; c++) {
					int num = a * 10000 + b * 1000 + c * 100 + b * 10 + a;
					op(num);
					num = a * 100000 + b * 10000 + c * 1100 + b * 10 + a;
					op(num);
				}
			}
		}
	}
	if (n >= 1e6 && m <= 1e8) {
		for (int a = 1; a <= 9; a += 2) {
			for (int b = 0; b <= 9; b++) {
				for (int c = 0; c <= 9; c++) {
					for (int d = 0; d <= 9; d++) {
						int num = a * 1000000 + b * 100000 + c * 10000 + d * 1000 + c * 100 + b * 10 + a;
						op(num);
						num = a * 10000000 + b * 1000000 + c * 100000 + d * 11000 + c * 100 + b * 10 + a;
						op(num);
					}
					
				}
			}
		}
	}
	rt();
	return 0;
}
