/*
问题描述：P0917
给你一个字符串 s ，根据下述规则反转字符串：
所有非英文字母保留在原有位置。
所有英文字母（小写或大写）位置反转。
返回反转后的 s 。
?
示例 1：
输入：s = "ab-cd"
输出："dc-ba"
示例 2：
输入：s = "a-bC-dEf-ghIj"
输出："j-Ih-gfE-dCba"
示例 3：
输入：s = "Test1ng-Leet=code-Q!"
输出："Qedo1ct-eeLg=ntse-T!"
?
提示
1 <= s.length <= 100
s 仅由 ASCII 值在范围 [33, 122] 的字符组成
s 不含 '\"' 或 '\\'
*/
class Solution {
public:
    bool isal(char c)
    {
        return (c>64&&c<91 || c>96&&c<123);
    }
    string reverseOnlyLetters(string s) {
        int i=0,j=s.size()-1;
        while (i<j)
        {
            if(isal(s[i])&&isal(s[j])) swap(s[i],s[j]);
            else if (isal(s[i])) i--;
            else if (isal(s[j])) j++;
            i++;j--;
        }
        return s;
    }
};