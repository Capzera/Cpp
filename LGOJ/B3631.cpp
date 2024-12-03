#include <bits/stdc++.h>
using namespace std;
struct ListNode {
	int val;
	ListNode *next;
} *head, *mp[1000010];
long long q, op, x, y;

void insert(int x, int y) {
	ListNode *p = mp[x];
	ListNode *q = new ListNode;
	q->val = y, q->next = p->next;
	p->next = q;
	mp[y] = q;
}

int query(int x) {
	ListNode *p = mp[x];
	ListNode *q = p->next;
	if (!q) return 0;
	return q->val;
}

void erase(int x) {
	ListNode *p = mp[x];
	ListNode *q = p->next;
	if (q) {
		p->next = q->next;
		delete q;
	}
}

int main() {
	cin >> q;
	head = new ListNode;
	head->val = 1, head->next = nullptr;
	mp[1] = head;
	while (q--) {
		cin >> op >> x;
		if (op == 1) {
			cin >> y;
			insert(x, y);
		}
		if (op == 2) {
			cout << query(x) << endl;
		}
		if (op == 3) {
			erase(x);
		}
	}
	delete head;
	return 0;
}
/*
mp[1] = ListNode *
mp[2] =
mp[99] =
*/
