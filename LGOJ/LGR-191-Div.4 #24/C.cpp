#include <bits/stdc++.h>
using namespace std;
int main() {
	int n;
	cin >> n;
	for (int i = 1; i <= n; i++) cout << 'Z';  // 第一行
	cout << endl;
	for (int i = 2; i <= n - 1; i++) { // 第2- n-1 行
		for (int j = 1; j <= n - i; j++) cout << '-'; 
		cout << 'Z';
		for (int j = 1; j <= i - 1; j++) cout << '-';
		cout << endl;
	}
	for (int i = 1; i <= n; i++) cout << 'Z'; // 第n行
	return 0;
}
