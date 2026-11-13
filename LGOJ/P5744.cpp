#include <bits/stdc++.h>
using namespace std;
long long n;
struct stu {
	string name;
	int age, score;
} x;
int main() {
	cin >> n;
	for (int i = 0; i < n; i++) {
		cin >> x.name >> x.age >> x.score;
		x.age++;
		x.score = min(6 * x.score / 5, 600);
		cout << x.name << " " << x.age << " " << x.score << endl;
	}
	return 0;
}
