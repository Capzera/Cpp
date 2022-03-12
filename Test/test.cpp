/*\1-3-1\Ë¼¿¼£ºvoid insertion_sort(Type *a, int n)

{
    Type key;                 //   cost  times
    for (i=1;i<n;i++)         //   c1    n  
    {
        key=a[i];             //   c2    n-1
        j=i-1;                //   c3    n-1
        while(j>=0&&a[j]>key) //   c4    sum of ti
        {
            a[j+1]=a[j];      //   c5    sum of (ti-1) 
            j--;              //   c6    sum of (ti-1)
        }
        a[j+1]=key;           //   c7    n-1
    }
}³£
*/
#include<bits/stdc++.h>
using namespace std;
    vector<int> nums;
    int n,p;
void insertionsort(vector<int>& nums)
{
    for(int i=1;i<n;i++)
    {
        int key=nums[i],j=i-1;
        while(j>=0&&nums[j]>key)
        {
            nums[j--+1]=nums[j];
        }
        nums[j+1]=key;
    }
}
int main()
{

    cin>>n;
    nums.resize(n);
    for(int i=0;i<n;i++)
    {
        cin>>p;
        nums[i]=p;
    }
    insertionsort(nums);
    for(int i:nums) cout<<i<<" ";
    system("pause");
    return 0;
}