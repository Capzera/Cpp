#include <bits/stdc++.h>
using namespace std;
string pre, mid, post;
void PostConstruct(string pre, string mid) {
	if (pre == "") return;
	char root = pre[0];
	int k = mid.find(root);
	string lpre = pre.substr(1, k), rpre = pre.substr(k + 1);
	string lmid = mid.substr(0, k), rmid = mid.substr(k + 1);
	PostConstruct(lpre, lmid);
	PostConstruct(rpre, rmid);
	cout << root;
}
int main() {
	cin >> mid >> pre;
	PostConstruct(pre, mid);
	return 0;
}
