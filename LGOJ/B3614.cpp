#include <bits/stdc++.h>
using namespace std;
string op;
unsigned long long t, n, x;
int main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	cin >> t;
	while (t--) {
		cin >> n;
		vector<unsigned long long> stk;
		while (n--) {
			cin >> op;
			if (op == "push") {
				cin >> x;
				stk.push_back(x);
			}
			if (op == "pop") {
				if (!stk.empty()) {
					stk.pop_back();
				} else {
					cout << "Empty" << endl;
				}
			}
			if (op == "query") {
				if (!stk.empty()) {
					cout << stk.back() << endl;
				} else {
					cout << "Anguei!" << endl;
				}
			}
			if (op == "size") {
				cout << stk.size() << endl;
			}
		}
	}
}
