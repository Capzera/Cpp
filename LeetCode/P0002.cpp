/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode() : val(0), next(nullptr) {}
 *     ListNode(int x) : val(x), next(nullptr) {}
 *     ListNode(int x, ListNode *next) : val(x), next(next) {}
 * };
 */
class Solution {
public:
    ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
        ListNode *p=l1,*q=l2,*ans=new ListNode(-1),*r;
        int x=0,len1=1,len2=1,i,add;
        p=l1;q=l2;
        while(p->next!=NULL)
        {
            len1++;
            p=p->next;
        }
        while(q->next!=NULL)
        {
            len2++;
            q=q->next;
        }
        if (len1<len2)
        {
            for(i=1;i<=len2-len1;i++)
            {
                p->next=new ListNode(0);
                p=p->next;
            }
        }
        else
        {
            for(i=1;i<=len1-len2;i++)
            {
                q->next=new ListNode(0);
                q=q->next;
            }
        }
        p=l1;q=l2;r=ans;
        while(p!=NULL&&q!=NULL)
        {
            add=p->val+q->val+x;
            if(add<10)
            {
                r->next=new ListNode(add);
                x=0;
            }
            else
            {
                r->next=new ListNode(add-10);
                x=1;
            }
            p=p->next;
            q=q->next;
            r=r->next;
        }
        if (x==1) r->next=new ListNode(1);
        r=ans;
        return r->next;
    }
};