class Solution {
public:
    int mySqrt(int x) {
        long long i;
        for(i=0;i<=x;i++)
        {
            if ((i*i<=x)&&((i+1)*(i+1)>x))return i;
        }
        return 0;
    }
};