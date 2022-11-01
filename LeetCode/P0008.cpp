class Solution {
public:
    int judge(char c)
    {
        if ((c>=48)&&(c<=57)) return 1;//数字
        if ((c==43)||(c==45)) return 2;//正负号
        if ((c>=65&&c<=90)||(c>=97&&c<=122)||(c==46)) return 3;//字母和点
        if (c==32) return 4;//空格
        return 0;
    }
    int myAtoi(string s) {
        int len=s.length(),i=0,minus=0,mark1=1,mark2=0;
        long long ans=0;
        while (judge(s[i])==4) i++;
        while (i<len)
        {
            if ((mark2==0)&&((judge(s[i])==3)||(judge(s[i])==2&&judge(s[i+1])!=1))) return 0;
            if (mark2==1&&judge(s[i])!=1) break;
            if (((judge(s[i])==2&&judge(s[i+1])==1)||(judge(s[i])==1))&&mark2==0) 
            {
                if (s[i]==45) minus=1;
                mark2=1;
            }
            if (mark2==1&&judge(s[i])==1) 
            {
                ans=ans*10+s[i]-48;
                if (ans>2147483648) break;
            }
            i++;
        }
        if (minus==1) 
        {
            if (ans>2147483648) ans=2147483648;
            return ans-2*ans;   
        }
        else 
        {
            if (ans>2147483647) ans=2147483647;
            return ans;
        }
    }
};