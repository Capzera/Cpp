#include<bits/stdc++.h>
using namespace std;
int main()
{
    queue<int> nums;
    int i,put,n;
    scanf("%d",&n);
    for(i=0;i<n;i++)
    {
        scanf("%d",&put);
        nums.push(put);
    }
    for(i=0;i<n;i++) 
    {
        printf("%d ",nums.front());
        nums.pop();
    }
    system("pause");
    return 0;
}