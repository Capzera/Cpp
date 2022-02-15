class Solution {
public:
    int reverse(int x) {
        int mark=0;
        long long ans=0;
        if (!x)return 0;
        if (x<0) 
        {
            mark=1;
            x=abs(x);
        }
        while(x>0)
        {
            ans=ans*10+(x%10);
            x/=10;
        }
        if (mark==1) ans-=2*ans;
        if (ans>=-2147483648&&ans<=2147483647)return ans;
        else return 0;
    }
};