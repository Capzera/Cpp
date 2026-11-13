#include<bits/stdc++.h>
using namespace std;
class StaticSearchTable {
private:
    int *data;
    int data_number;
    bool SqlList(int loc,int key);
    void qsort();
    bool flag;
public:
    StaticSearchTable(int n,bool flag);
    int SqlList(int key);
    int Search(int key);
    void print();
};//定义一个顺序表的类，这里我想用C++的方式来写
StaticSearchTable::StaticSearchTable(int n,bool flag)
{
    this->flag = flag;
    srand(time(NULL));
    data = new int[n + 1];
    for (int i = 0; i < n; i++) data[i + 1] = rand()%100;
    data_number = n + 1;
    if (flag) qsort();
    
}
int StaticSearchTable::Search(int key)//二分查找
{
    int low = 1;
    int high = data_number;
    int result = 0;
    while (low <= high)
    {
        int mid = (low + high) / 2;
        if (data[mid] == key)
        {
            result = mid;
            break;
        }
        else if (key < data[mid]) high = mid - 1;
        else low = mid + 1;
    }
    return result;
}
void QuickSort(int l,int r,int a[])
{
    if (l>r) return;
    int i=l,j=r,mid=(l+r)div 2;
    while(i<j)
    {
        while(a[i]<a[mid]&&i<j) i++;
        while(a[j]>a[mid]&&i<j) j--;
        if (i<j) swap(a[i],a[j]);
    }
    QuickSort(l,j,a);
    QuickSort(i,r,a);
}
int main(){
    system("pause");
    return 0;
}