#include <bits/stdc++.h>
using namespace std;
string s;
string scramberi = "UWYGADFPVZBECKMTHXSLRINQOJ";
string scramberii = "AJPCZWRLFBDKOTYUQGENHXMIVS";
string scramberiii = "TAGBPCSDQEUFVNZHYIXJWLRKOM";
string reflector = "YRUHQSLDPXNGOKMIEBFZCWVJAT";
int one(int start, char x) {
	int tgt = (x - 'A' + start) % 26;
	for (int k = 0; k < 26; k++) {
		if (scramberi[k] - 'A' == tgt) return k;
	}
	return -1;
}
int reflection(int start, int ptr) {
	ptr = (ptr - start + 26) % 26;
	ptr = reflector[ptr] - 'A';
	return (ptr + start + 26) % 26;
}
int main() {
	cin >> s;
	for (int ori = 0; ori < 26; ori++) {
		cout << "start = " << ori << " : ";
		for (int i = 0, start = ori; i < s.size(); i++, start++) {
			int ptr = one(start, s[i]);
			ptr = reflection(start, ptr);
			j = scramberi[ptr] - 'A';
			j = (j - start + 26) % 26;
			char ans = 'A' + j;
			cout << ans;
		}
		cout << '\n';
	}
}
/*
GYHRVFLRXY  AB SZ UY GH LQ EN
BLITZKRIEM
I  P  2  1  3  R  R  3  1  2  P  O
G->H->I->A->B->C->U->L->M->B->A->B
Y->U->W->H->N->M->O->H->B->S->Q->L
H->G->J->C->J->S->F->S->I->L->I->I
R->R->V->Y->Z->N->K->F->V->X->T->T
V->V->A->Z->G->B->R->X->U->X->S->Z
F->F->L->F->F->G->L->V->O->Q->K->K
L->Q->X->S->P->D->H->Q->L->Y->R->R
R->R->Z->A->B->C->U->L->M->Q->I->I
X->X->G->M->L->U->C->B->A->W->N->E
Y->U->E->M->L->U->C->B->A->R->H->G
  ABCDEFGHIJKLMNOPQRSTUVWXYZ
                      ↓
2 KLMNOPQRSTUVWXYZABCDEFGHIJ
2 DKOTYUQGENHXMIVSAJPCZWRLFB

1 EFGHIJKLMNOPQRSTUVWXYZABCD
1 ADFPVZBECKMTHXSLRINQOJUWYG

3 BCDEFGHIJKLMNOPQRSTUVWXYZA
3 AGBPCSDQEUFVNZHYIXJWLRKOMT

R ABCDEFGHIJKLMNOPQRSTUVWXYZ
R YRUHQSLDPXNGOKMIEBFZCWVJAT

ABCDEFGHIJKLMNOPQRSTUVWXYZ
12345678901234567890123456

E  = 5    E
EG = 12   L
G  = 7    G
A  = 1    AEnigma.cpp
GBDE = 18  R
EGCG = 22  V
EGCE = 20  T
E = 5      E
DGC = 14    N
ELGAR    VTEN



AA AG AC AT
GA GG GC GT
CA CG CC CT
TA TG TC TT


CG
*/ariavoltport\
The Oval Portrait
