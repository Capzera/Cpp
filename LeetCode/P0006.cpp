class Solution {
public:
    string convert(string s, int numRows) {
        string ans[1001];
        int i,x=0,row=0;
        for(i=0;i<=s.length();i++)
        {
            if (x==0) 
            {
                row++;
                if (row==numRows) x=1;
            }
            else
            {
                row--;
                if (row==1) x=0;
            }
            ans[row]+=s[i];
        }
        s="";
        for(i=1;i<=numRows;i++) s+=ans[i];
        return s;
    }
};