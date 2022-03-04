// The API isBadVersion is defined for you.
bool isBadVersion(int version);
class Solution {
public:
    int firstBadVersion(int n) {
        int l=1,r=n,mid=1+(n-1)/2;
        while (l<r)
        {
            if (isBadVersion(mid)) r=mid;
            else l=mid+1;
            mid=l+(r-l)/2;
        }
        return mid;
    }
};