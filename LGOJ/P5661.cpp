#include <bits/stdc++.h>
using namespace std;
const int N = 1e5 + 10;
struct node {
	int car, price, time;
} q[N];
int n, head = 1, car, price, ti, tail, ans;
int main() {
	cin >> n;
	for (int i = 1; i <= n; i++) {
		cin >> car >> price >> ti;
		if (!car) {
			q[++tail] = {car, price, ti};
			ans += price;
			continue;
		}
		while (head <= tail && ti - q[head].time > 45) head++;
		ans += price;
		for (int j = head; j <= tail; j++) {
			if (ti - q[j].time <= 45 && q[j].price >= price) {
				ans -= price;
				q[j].time = -45;
				break;
			}
		}
	}
	cout << ans;
	return 0;
}
