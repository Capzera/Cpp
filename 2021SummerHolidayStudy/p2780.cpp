#include <bits/stdc++.h>
using namespace std ;
    const int R = 60 ;
    const int MAXN = 609 ;
    const int MAXB = 180009 ;
    const int __MAXK = 13 ;
int n , k , a , b , x[MAXN] , k0 , sumx[MAXN] ;
long long F[MAXB][__MAXK] ;
void Init() {
freopen("game.in","r",stdin);
freopen("game.out","w",stdout);
    cin >> n >> k >> a >> b ;
    for (int i = 1 ; i <= n ; i ++ ) cin >> x[i] ;
    sumx[0] = 0 ;
    for (int i = 1 ; i <= n ; i ++ ) sumx[i] = sumx[i-1] + x[i] ;
}
void Solve() {
    k0 = (k+1) / R ;
    if ( R * k0 < k+1 ) k0 ++ ;
    for (int j = 0 ; j <= b ; j ++ )
        for (int k = 0 ; k < k0 ; k ++ )
            F[j][k] = 0 ;
    F[0][0] = 1 ;
    for (int i = 1 ; i <= n ; i ++ )
        for (int j = sumx[i] ; j >= 0 ; j -- )
            for (int k = 0 ; k < k0 ; k ++ ) {
                if ( k + 1 < k0 ) F[j+x[i]][k+1] |= (F[j][k] >> (R-1)) ;
                F[j+x[i]][k] |= (F[j][k] << 1) ;
            }
    vector<int> ans ; ans.clear() ;
    for (int j = 0 ; j <= b ; j ++ )
        if ( ((F[j][k0-1] >> (k+1 - R*(k0-1) - 1)) & 1) == 1 )
            ans.push_back(j) ;
    int ret = 0 ;
    int near2a = abs(ans[0] - a) , near2b = abs(ans[0] - b) ;
    for (int i = 1 ; i < ans.size() ; i ++ ) {
        if (abs(ans[i] - a) < near2a ) near2a = abs(ans[i] - a) ;
        if (abs(ans[i] - b) < near2b ) near2b = abs(ans[i] - b) ;
        int rr = (ans[i] - ans[i-1]) / 2 ;
        if ( (a <= ans[i-1] + rr && ans[i-1] + rr <= b ) || ( a <= ans[i] - rr && ans[i] - rr <= b ) ) {
            if (rr > ret ) ret = rr ;
        }
    }
    if (near2a > ret ) ret = near2a ;
    if (near2b > ret ) ret = near2b ;
    cout << ret << "\n" ;
    fprintf(stderr,"%d\n" , ret) ;
}
int main() {
    Init() ;
    Solve() ;
}