/*
给定两个字符串 s 和 t ，编写一个函数来判断 t 是否是 s 的字母异位词。
注意：若?s 和 t?中每个字符出现的次数都相同，则称?s 和 t?互为字母异位词。
示例?1:
输入: s = "anagram", t = "nagaram"
输出: true
示例 2:
输入: s = "rat", t = "car"
输出: false
*/
class Solution {
public:
    bool isAnagram(string s, string t) {
        unordered_map<char,int> m1,m2;
        if (s.size()!=t.size()) return false;
        int i,n=s.size();
        for(i=0;i<n;i++)
        {
            m1[s[i]]++;//记录s字符串字母的哈希表
            m2[t[i]]++;//记录t字符串字母的哈希表
        }
        for(i=97;i<=122;i++)
        {
            if (m1[i]!=m2[i]) return false;//哈希表不同
        }
        return true;
    }
};