/*
问题描述：
387. 字符串中的第一个唯一字符
给定一个字符串 s ，找到 它的第一个不重复的字符，并返回它的索引 。如果不存在，则返回 -1 。
示例 1：
输入: s = "leetcode"
输出: 0
示例 2:
输入: s = "loveleetcode"
输出: 2
示例 3:
输入: s = "aabb"
输出: -1
*/
class Solution {
public:
    int firstUniqChar(string s) {
        unordered_map<char,int> um;//本题可以利用unordered_map的存储方式为FILO的方式来做
        vector<int> ab(27);//保存每个字符出现的位置，如果为第一次出现，则不会被二次修改
        int n=s.size(),i;
        for(i=n-1;i>=0;i--)//倒序循环，将字符压入哈希表
        {
            um[s[i]]++;
            ab[s[i]-96]=i;
        }
        for(auto& m:um) 
        {
            if (m.second==1) return ab[m.first-96];//如果哈希表保存的次数为1就返回哈希表最顶上元素的位置
        }
        return -1;//循环做完依然未找到，说明所有字母出现次数均大于1
    }
};