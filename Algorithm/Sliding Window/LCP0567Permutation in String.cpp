/*
问题描述：
给你两个字符串?s1?和?s2 ，写一个函数来判断 s2 是否包含 s1?的排列。如果是，返回 true ；否则，返回 false 。
换句话说，s1 的排列之一是 s2 的 子串 。
示例 1：
输入：s1 = "ab" s2 = "eidbaooo"
输出：true
解释：s2 包含 s1 的排列之一 ("ba").
示例 2：
输入：s1= "ab" s2 = "eidboaoo"
输出：false
*/
class Solution {
public:
    bool checkInclusion(string s1, string s2) {
        unordered_map<char,int> um;
        int n1=s1.size(),n=s2.size(),i;
        if (n1>n) return false;//特判,s1比s2长，一定不匹配
        for(i=0;i<n1;i++) um[s1[i]]++;//创建s1哈希表
        i=0;//初始化指针
        unordered_map<char,int> compare;//创建滑动窗口
        for(i=0;i<n1;i++) compare[s2[i]]++;//将s2中前n1个字符压入窗口
        for(i=n1;i<n;i++)//开始滑动
        {
            if (compare==um) return true;//如果窗口内元素等于哈希表，匹配成功
            compare[s2[i]]++;//滑动窗口压入一个元素
            if (compare[s2[i-n1]]>1) compare[s2[i-n1]]--;//如果需要释放的元素个数大于1，个数-1
            else compare.erase(s2[i-n1]);//待释放元素仅有一个，将其从窗口擦除
        }
        return compare==um;
    }
};