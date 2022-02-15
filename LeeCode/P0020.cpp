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