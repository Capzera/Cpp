/*
问题描述：P0005
给你一个字符串 s，找到 s 中最长的回文子串。
示例 1：
输入：s = "babad"
输出："bab"
解释："aba" 同样是符合题意的答案。
示例 2：
输入：s = "cbbd"
输出："bb"
提示：
1 <= s.length <= 1000
s 仅由数字和英文字母组成
*/
class Solution {
public:
    string longestPalindrome(string s) {
        int n=s.size(),i,j,maxlen=1;
        if (n<2) return s;
        string ans="";
        ans+=s[0];
        for(int L=1;L<3;L++)//L控制初始串长度
            for(i=0;i<n-L+1;i++)//i为回文串的初始中心
            {
                if (L==2&&s[i]!=s[i+1]) continue;//回文串基础从2开始，必须保证这两个相等
                for (j=0;j<n/2+1;j++)//j控制回文串的拓展长度
                {
                    int l=i-j,r=i+j+L-1;
                    if (l<0||r>=n||s[l]!=s[r]) break;//越界、不回文退出
                    if (r-l+1>maxlen)
                    {
                        maxlen=r-l+1;//保存最大长度
                        ans=s.substr(l,r-l+1);//截取答案
                    }
                }
            }
        return ans;
    }
};