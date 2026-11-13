#include <bits/stdc++.h>
using namespace std;
long long n;
struct stu {int id, sco1, sco2;} x;
int main() {
	cin >> n;
	for (int i = 0; i < n; i++) {
		cin >> x.id >> x.sco1 >> x.sco2;
		if (x.sco1 + x.sco2 > 140 && 7 * x.sco1 + 3 * x.sco2 >= 800) cout << "Excellent\n";
		else cout << "Not excellent\n";
	}
	return 0;
}
