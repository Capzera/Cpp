class Solution {
public:
    int guessNumber(int n) {
        int m=2;
        long long mid=0,l=1,r=n;
        while (m!=0)
        {
            mid=(l+r)/2;
            m=guess(mid);
            if (m==-1) r=mid-1;
            else if (m==1) l=mid+1;
            else if (m==0) break;
        }
        return mid;
    }
};