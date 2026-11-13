/*
问题描述：P0023
给你一个链表数组，每个链表都已经按升序排列。
请你将所有链表合并到一个升序链表中，返回合并后的链表。
示例 1：
输入：lists = [[1,4,5],[1,3,4],[2,6]]
输出：[1,1,2,3,4,4,5,6]
解释：链表数组如下：
[
  1->4->5,
  1->3->4,
  2->6
]
将它们合并到一个有序链表中得到。
1->1->2->3->4->4->5->6
示例 2：
输入：lists = []
输出：[]
示例 3：
输入：lists = [[]]
输出：[]
提示：
k == lists.length
0 <= k <= 10^4
0 <= lists[i].length <= 500
-10^4 <= lists[i][j] <= 10^4
lists[i] 按 升序 排列
lists[i].length 的总和不超过 10^4
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
    ListNode* mergeKLists(vector<ListNode*>& lists) {
        int n=lists.size(),tot=0;
        ListNode* ans=new ListNode(-1);
        ListNode* p=ans;
        vector<ListNode*> pointer(n);
        for(int i=0;i<n;i++) 
        {
            pointer[i]=lists[i];
            ListNode* q=lists[i];
            while (q!=NULL)
            {
                tot++;
                q=q->next;
            }
        }
        for(int i=0;i<tot;i++)
        {
            int min=INT_MAX,minloc=-1;
            for(int j=0;j<n;j++)
            {
                if (!lists[j]||lists[j]->val>=min) continue;
                if (lists[j]->val<min)
                {
                    min=lists[j]->val;
                    minloc=j;
                }
            }
            ListNode* q=new ListNode(min);
            ans->next=q;
            ans=ans->next;
            lists[minloc]=lists[minloc]->next;
        }
        return p->next;
    }
};