/*
问题描述：
给定一个字符串 s ，请你找出其中不含有重复字符的?最长子串?的长度。
示例?1:
输入: s = "abcabcbb"
输出: 3 
解释: 因为无重复字符的最长子串是 "abc"，所以其长度为 3。
示例 2:
输入: s = "bbbbb"
输出: 1
解释: 因为无重复字符的最长子串是 "b"，所以其长度为 1。
示例 3:
输入: s = "pwwkew"
输出: 3
解释: 因为无重复字符的最长子串是?"wke"，所以其长度为 3。
?    请注意，你的答案必须是 子串 的长度，"pwke"?是一个子序列，不是子串。
*/
class Solution {
public:
    int lengthOfLongestSubstring(string s) {
        int i=0,j=0,n=s.size(),ans=1;
        if (n==0) return 0;//空字符串子串均为0
        if (n==1) return 1;//特殊情况判断
        unordered_map<char,int> um;
        um[s[0]]=0;
        while (i<n-1&&j<n)
        {
            j++;//右边界向右滑动
            if(um.find(s[j])!=um.end()) //判断如果出现重复字母
            {
                ans=max(ans,j-i);//保存最大子串长度
                i=um[s[j]]+1;//左边界置为重复字符的右一个
                j=i;//边界归零
                um.clear();//清空哈希表
                um[s[i]]=i;//记录起点
            }
            else um[s[j]]=j;//窗口继续滑动
        }
        ans=max(ans,j-i);//判断如果子串在末尾的情况（j，i符合循环结束条件但是未保存长度）
        return ans;
    }
};