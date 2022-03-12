\1-3-1\Ë¼¿¼£ºvoid insertion_sort(Type *a, int n)

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
}
1 2 3 4 5 6 7 8
