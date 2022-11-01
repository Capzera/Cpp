class Solution {
public:
    bool detectCapitalUse(string word) {
        int i,mark1=0,mark2=0;
        for(i=0;i<word.length();i++)
        {
            if (word[i]>=97) mark1=1;
            if (word[i]<=90&&i>0) mark2=1;
            if (mark1+mark2==2) return false;
        }
        return true;
    }
};