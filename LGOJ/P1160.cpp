#include <bits/stdc++.h>
using namespace std;
struct ListNode {
    ListNode *next, *prev;
    int val;
    ListNode (int _val) : val(_val), next(nullptr), prev(nullptr) {}
};
int main() {
    unordered_map<int, ListNode*> mp;
    unordered_set<int> s;
    ListNode *dummy = new ListNode(-1);
    ListNode *head = new ListNode(1);
    dummy->next = head, head->prev = dummy;
    mp[1] = head;
    int n, m, k, v;
    cin >> n;
    for (int i = 2; i <= n; i++) {
        cin >> k >> v;
        ListNode *p = mp[k], *q = new ListNode(i);
        if (!v) p = p->prev;
        q->prev = p, q->next = p->next;
        if (p->next) p->next->prev = q;
        p->next = q;
        mp[i] = q;
    }
    cin >> m;
    while (m--) {
        cin >> k;
        s.emplace(k);
    }
    ListNode *p = dummy->next;
    while (p) {
        if (!s.count(p->val)) cout << p->val << " ";
        p = p->next;
    }
    system("pause");
    return 0;
}