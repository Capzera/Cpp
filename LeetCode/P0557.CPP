/*
题目描述：
    给定一个字符串?s?，你需要反转字符串中每个单词的字符顺序，同时仍保留空格和单词的初始顺序。
示例 1：
输入
    s = "Let's take LeetCode contest"
输出
    "s'teL ekat edoCteeL tsetnoc"
示例 2:
输入： 
    s = "God Ding"
输出：
    "doG gniD"
*/
class Solution {
public:
    string reverseWords(string s) {
        string ans="",temp="";
        int n=s.length(),i,op,ed,state=0;
        if (s.empty()||n==1) return s;
        s.push_back(' ');
        for(i=0;i<=n;i++)
        {
            if (s[i]==' ')
            {
                state=1;//state=1 录入单词
                reverse(temp.begin(),temp.end());//反转录入的单词
                ans+=temp;
                temp="";
                if (i!=n) ans+=' ';//排除最后一个空格，录入空格
            }
            else if (state) state=0;//state=0录入状态
            if (!state) temp.push_back(s[i]);//当前为单词内，压入单词栈
        }
        return ans;
    }
};