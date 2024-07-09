#include <iostream>  
using namespace std;  
  
int main() {  
    int M, t = 0, cd = 1, ds = 0; 
    cin >> M;  
  
    for (int d = 1; d <= M; ++d) {  
        if (ds == cd) {  
            ++cd;  
            ds = 0;  
        }  
        t += cd;  
        ++ds;  
    }  
  
    cout <<t<< endl;  
  
    return 0;  
}
