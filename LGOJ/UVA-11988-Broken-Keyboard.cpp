#include <bits/stdc++.h>
using namespace std;
int main() {
    char s[100001];
    while (cin >> s) {
        int len = strlen(s), tail = 0, p = 0;
        int next[len + 1] = {0};
        for (int i = 1; i <= len; i++) {
            if (s[i - 1] == '[') p = 0;
            else if (s[i - 1] == ']') p = tail;
            else {
                next[i] = next[p];
                next[p] = i;
                p = i;
                if (!next[p]) tail = p;
            }
        }
        for (int i = next[0]; i; i = next[i]){
            cout << s[i - 1];
        }
        cout << endl;
    }
    return 0;
}
/*ab[c]d[e]
#include <bits/stdc++.h>
using namespace std;
struct ListNode {
    char val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(char x) : val(x), next(nullptr) {}
    ListNode(char x, ListNode *next) : val(x), next(next) {}
}*head, *tail, *p;


int main() {
    string s;
    while (cin >> s) {
        head = tail = p = new ListNode();
        for (auto& ch : s) {
            if (ch == '[') p = head;
            else if (ch == ']') p = tail;
            else {
                ListNode *q = new ListNode(ch, p->next);
                p->next = q;
                p = q;
                if (!p->next) tail = p;
            }
        }
        for (p = head->next; p; p = p->next) cout << p->val;
        cout << endl;
    }
    system("pause");
    return 0;
}
*/