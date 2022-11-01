/*
问题描述：P0020 有效地括号
给定一个只包括 '('，')'，'{'，'}'，'['，']'?的字符串 s ，判断字符串是否有效。
有效字符串需满足：
左括号必须用相同类型的右括号闭合。
左括号必须以正确的顺序闭合。
示例 1：
输入：s = "()"
输出：true
示例?2：
输入：s = "()[]{}"
输出：true
示例?3：
输入：s = "(]"
输出：false
示例?4：
输入：s = "([)]"
输出：false
示例?5：
输入：s = "{[]}"
输出：true
提示：
1 <= s.length <= 104
s 仅由括号 '()[]{}' 组成
*/
class Solution {
public:
    bool isValid(string s) {
        stack<int> brackets;
        int length=s.length(),i;
        for(i=0;i<length;i++)
        {
            if (!brackets.size()||s[i]=='('||s[i]=='['||s[i]=='{') brackets.push(s[i]);
            else switch (s[i])
            {
                case ')':
                    if (brackets.size()>0&&brackets.top()=='(') brackets.pop();
                    else return false;
                break;
                case ']':
                    if (brackets.size()>0&&brackets.top()=='[') brackets.pop();
                    else return false;
                break;
                case '}':
                    if (brackets.size()>0&&brackets.top()=='{') brackets.pop();
                    else return false;
                break;
            }
        }
        return brackets.size()==0; 
    }
};