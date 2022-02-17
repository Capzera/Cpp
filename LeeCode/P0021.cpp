/*
问题描述：
将两个升序链表合并为一个新的 升序 链表并返回。新链表是通过拼接给定的两个链表的所有节点组成的。?
示例 1：
输入：l1 = [1,2,4], l2 = [1,3,4]
输出：[1,1,2,3,4,4]
示例 2：
输入：l1 = [], l2 = []
输出：[]
示例 3：
输入：l1 = [], l2 = [0]
输出：[0]
*/
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