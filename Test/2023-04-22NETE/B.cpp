#pragma GCC optimize(2)
#include <bits/stdc++.h>
using namespace std;
struct ListNode {
    int id, plane, sleep;
    ListNode *next;
    ListNode(int _id = 0, int _plane = 0) : id(_id), plane(_plane), sleep(0), next(nullptr) {}
};
class base {
private:
    int plane, maxPlane;
    unordered_map<int, int> mp;
    ListNode *head, *tail;
    queue<int> q;
public:
    base(int _c) : plane(_c), maxPlane(_c) {
        head = tail = new ListNode(-1, -1);
    }
    int operat () {
        if (q.empty()) return 0;
        int ans = 0, id = q.front(), num = mp[id];
        if (num <= plane || plane == maxPlane) {
            tailIns(id, num);
            plane -= num;
            ans += num;
            q.pop();
        }
        if (plane == maxPlane && plane < num) {
            maxPlane = num;
            plane = 0;
        }
        return ans;
    }
    void send(int id, int num) {
        mp[id] = num;
        q.push(id);
        cout << operat() << endl;
    }

    void callBack(int id) {
        setCallBack(id);
        remove();
        int ans = 0, ret = operat();
        while (ret) {
            ans += ret;
            ret = operat();
        }
        cout << ans << endl;
    }

    void tailIns(int id, int num) {
        tail->next = new ListNode(id, num);
        tail = tail->next;
    }

    void setCallBack(int id) {
        ListNode *p = head->next;
        while (p->id != id) {
            p = p->next;
        }
        p->sleep = 1;
    }

    void remove() {
        ListNode *p = head->next;
        while (p && p->sleep) {
            plane += p->plane;
            p = p->next;
        }
        head->next = p;
        if (!head->next) tail = head;
    }
};
int main() {
    int c, n, id, num;
    cin >> c >> n;
    base g(c);
    for (int i = 0; i < n; i++) {
        cin >> id >> num;
        if (id < 0) g.callBack(num);
        else g.send(id, num); 
    }
    system("pause");
    return 0;
}
/*
3 5
0 2
1 2
-1 0
2 4
-1 1

10 9
0 3
1 2
2 4
-1 0
-1 2
3 2
4 3
-1 1
5 5
*/