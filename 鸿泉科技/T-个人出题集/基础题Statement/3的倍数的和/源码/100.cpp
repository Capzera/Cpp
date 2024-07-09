#include <iostream>
using namespace std;
int n, sum;
int a[1010];
int main() {
	cin >> n;
	for (int i = 0; i < n; i++) {
		cin >> a[i];
	}
	for (int i = n - 1; i >= 0; i--) {
		cout << a[i] << " ";
	}
	return 0;
}
/*
5
1 5 2 4 3

3 4 2 5 1
数组，同时定义多个相同类型的变量
*/
