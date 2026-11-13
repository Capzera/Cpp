#include <bits/stdc++.h>
using namespace std;
struct ListNode {
	int val; // value 当前结点内部的信息
	ListNode *prev = nullptr, *next = nullptr;
	ListNode() {}
	ListNode(int _v) : val(_v) {}
	ListNode(int _v, ListNode* _n, ListNode* _p) : val(_v), next(_n), prev(_p) {}
} *head, *tail, *mp[500001];
int n, q, op, x, y;
void Del(int x) {
	if (!mp[x]) return;
	ListNode *p = mp[x];
	p->prev->next = p->next;
	p->next->prev = p->prev;
	mp[x] = nullptr;
}
void LeftIns(int x, int y) {
	if (x == y) return;
	ListNode *p = mp[y], *q = mp[x]; // q插到p的左边
	Del(x);
	mp[x] = q;
	q->next = p;
	q->prev = p->prev;
	p->prev->next = q;
	p->prev = q;
}
void RightIns(int x, int y) {
	if (x == y) return;
	ListNode *p = mp[y], *q = mp[x];
	Del(x);
	mp[x] = q;
	q->prev = p;
	q->next = p->next;
	p->next->prev = q;
	p->next = q;
}
void print() {
	ListNode *p = head->next;
	if (p == tail) {
		cout << "Empty!" << endl;
		return;
	}
	while (p != tail) {
		cout << p->val << " ";
		p = p->next;
	}
	cout << endl;
}
int main() {
	cin >> n >> q;
	head = new ListNode(-1);
	tail = new ListNode(-1);
	head->next = tail, tail->prev = head;
	for (int i = 1; i <= n; i++) {
		ListNode *p = new ListNode(i);
		p->prev = tail->prev;
		p->next = tail;
		tail->prev->next = p;
		tail->prev = p;
		mp[i] = p;
	}
	while (q--) {
		cin >> op >> x;
		if (op == 1) {
			cin >> y;
			LeftIns(x, y);
		}
		if (op == 2) {
			cin >> y;
			RightIns(x, y);
		}
		if (op == 3) {
			Del(x);
		}
	}
	print();
	return 0;
}
