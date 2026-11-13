#include <bits/stdc++.h>
using namespace std;
int main() {
	int n, op, x;
	cin >> n;
	priority_queue<int, vector<int>, greater<int> > pq;
	while (n--) {
		cin >> op;
		if (op == 1) {
			cin >> x;
			pq.push(x);
		}
		if (op == 2) {
			cout << pq.top() << endl;
		}
		if (op == 3) {
			if (pq.size()) pq.pop();
		}
	}
	return 0;
}
