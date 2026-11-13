#include<bits/stdc++.h>
using namespace std;
const int maxn = 1000010;
int n;
char s[maxn];
int sa[maxn],srank[maxn],height[maxn];
int p[maxn] , tmp[maxn] , cnt[maxn];
bool equ(int x,int y,int l){
    return srank[x] == srank[y] && srank[x + l] == srank[y + l];
}
void doubling(){
    for(int i = n;i;--i) s[i] = s[i - 1];
    for(int i = 1;i <= n;++i){
        srank[i] = s[i];
        sa[i] = i;
    }
    for(int l = 0 , pos = 0 , sig = 255;pos < n;sig = pos){
        pos = 0;
        for(int i = n - l + 1;i <= n;++i) p[++pos] = i;
        for(int i = 1;i <= n;++i)
        	if(sa[i] > l) p[ ++pos ] = sa[i] - l;
        memset( cnt , 0 ,sizeof(int) * (sig + 1));
        for(int i = 1;i <= n;++i) ++cnt[srank[i]];
        for(int i = 1;i <= sig;++i) cnt[i] += cnt[i - 1];
        for(int i = n;i;--i) sa[cnt[srank[p[i]]] -- ] = p[i];
        pos = 0;
        for(int i = 1;i <= n;++i) tmp[sa[i]] = equ(sa[i] , sa[i - 1] , l)?pos:++pos;
        for(int i = 1;i <= n;++i) srank[i] = tmp[i];
        l = !l? 1 : l << 1;
		for (i = 1, j = 0; i <= n; ++i) {  // Çó height
    		if (j) --j;
    		while (a[i + j] == a[sa[rk[i] - 1] + j]) ++j;
    		ht[rk[i]] = j;
  		}
    }
    return;
}
int main(){
    scanf("%s",s);
    n = strlen(s);
    doubling();
    for(int i = 1;i <= n;++i) printf("%d ",sa[i]);
	system("pause");
    return 0;
}