#include <bits/stdc++.h>
using namespace std;
long long a, b, c, d;
int main() {
	cin >> a >> b >> c >> d;
	cout << min({a, b + c, b + d});
	return 0;
}
