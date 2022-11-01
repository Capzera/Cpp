class Solution {
public:
    int romanToInt(string s) {
    int i=0,len=s.length(),ans=0;
        while (i<len)
        {
            if (s[i]=='M') 
            {
                ans+=1000;
                i++;
            }
            else if (s[i]=='D') 
            {
                ans+=500;
                i++;
            }
            else if ((s[i]=='C')&&((s[i+1]=='D')||(s[i+1]=='M')))
            {
                if (s[i+1]=='D')
                {
                    ans+=400;
                    i+=2;
                }
                else
                {
                    ans+=900;
                    i+=2;
                }
            }
            else if (s[i]=='C')
            {
                ans+=100;
                i++;
            }
            else if (s[i]=='L')
            {
                ans+=50;
                i++;
            }
            else if ((s[i]=='X')&&((s[i+1]=='L')||(s[i+1]=='C')))
            {
                if (s[i+1]=='L')
                {
                    ans+=40;
                    i+=2;
                }
                else
                {
                    ans+=90;
                    i+=2;
                }
            }
            else if (s[i]=='X')
            {
                ans+=10;
                i++;
            }
            else if (s[i]=='V')
            {
                ans+=5;
                i++;
            }
            else if ((s[i]=='I')&&((s[i+1]=='V')||(s[i+1]=='X')))
            {
                if  (s[i+1]=='V')
                {
                    ans+=4;
                    i+=2;
                }
                else
                {
                    ans+=9;
                    i+=2;
                }
            }
            else
            {
                ans+=1;
                i++;
            }
        }
        return ans;
    }
};