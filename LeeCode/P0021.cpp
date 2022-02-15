class Solution {
public:
    ListNode* mergeTwoLists(ListNode* l1, ListNode* l2) {
        ListNode *l3=new ListNode(-1);
        ListNode *r=l3;
        while (l1!=NULL&&l2!=NULL)
        {
            if (l1->val<l2->val)
            {
                r->next=l1;
                l1=l1->next;
            }
            else
            {
                r->next=l2;
                l2=l2->next;
            }
            r=r->next;
        }
        if (l1!=NULL)r->next=l1;
        else r->next=l2;
        return l3->next;
    }
};