/*
问题描述：
给你一个链表的头节点 head 和一个整数 val ，请你删除链表中所有满足 Node.val == val 的节点，并返回 新的头节点 。
示例 1：
输入：head = [1,2,6,3,4,5,6], val = 6
输出：[1,2,3,4,5]
示例 2：
输入：head = [], val = 1
输出：[]
示例 3：
输入：head = [7,7,7,7], val = 7
输出：[]
*/
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
    ListNode* removeElements(ListNode* head, int val) {
        while (head!=NULL&&head->val==val) head=head->next;//处理头结点为目标数的情况
        ListNode* p=head;
        ListNode* r;
        while (p!=NULL)
        {
            if (p->val!=val) r=p;//记录上一个合法地址
            else r->next=p->next;//修改上一个合法地址的next
            p=p->next;//指针前滚
        }
        return head;
    }
};