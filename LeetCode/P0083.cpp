class Solution {
public:
    ListNode* deleteDuplicates(ListNode* head) {
        if (!head) return head;
        ListNode *p=head,*r;
        while (p->next!=NULL) 
        {
            r=p->next;
            if (p->val==r->val) 
            {
                p->next=r->next;
                delete r;
            }
            else p=r;
        }
        return head;
    }
};