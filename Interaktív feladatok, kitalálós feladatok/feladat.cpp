#include "query.h"
#define MaxN            30000    //max. m�ret
#define MaxK            20       //MaxN<=2^MaxK
int main(){
  int N;             //a tanul�k sz�ma
  int M;             //az aktu�lis elemsz�m
  int Fel;           //az aktu�lis elemsz�m fele
  bool B[MaxK];      //B[k]=true, akkor �s csak akkor, ha van 2^k elemsz�m� r�szhalmaz
  int Rep[MaxK];     //Rep[k] a 2^k elemsz�m� r�szhalmaz egy eleme
  int Pow2[MaxK];    //Pow2[k]=2^k
  int L;             //a legnagyobb elemsz�m� r�szhalmaz elemsz�ma 2^L
  int i, k;
  Pow2[0] = 1;
  for (k = 1; k <= MaxK; k++){
    Pow2[k] = Pow2[k-1] << 1;
    B[k]=false;
  }
  N = Size();
  M = N ;
  Fel = M/2 + 1;
  L = 0;
  i = 0;
  while (i < N){
    k = 0;
    B[0] = true;
    Rep[0] = ++i;
    i++;   //
    if (i > N) break;
    while (B[k]){                //van k�t 2^k elemsz�m� r�szhalmaz
      if (Member(Rep[k],i)==1){  //egyes�ts�k a k�t 2^k elemsz�m� r�szhalmazt
         B[k] = false;
         k++;
         if (k>L) L=k;           //�j legnagyobb elemsz�m� r�szhalmaz
      }else{
         M -= Pow2[k+1];            //M:=M-2^(k+1)
         Fel -= Pow2[k];            //Fel:=Fel-2^k
         B[k] = false;              //t�r�lj�k a r�szhalmazt
         if (k==L)                  //L aktualiz�l�sa
            while (L>0 && !B[L]) L--;
         k = -1;
         break;
      }
    }//while
    if (k>=0) {
      B[k] = true;               //form a new subgroup having 2^k elements
      Rep[k] = i;                //i az �j r�szhalmaz reprezent�l�sa
    }
    if (L>0 && Pow2[L]>=Fel)     //van el�g ismeret�nk
      break;
  }
  Answer(Rep[L]);                //a legnagyobb r�szhalmaz egy eleme a megold�s
}//main
//az al�bbi sorokat ments�k el a query.h f�jlba
extern int Size(void);
extern int Member(int i, int j);
extern void Answer(int i);
//az al�bbi sorokat ments�k el a query.cpp f�jlba
#include <iostream>
#include <stdlib.h>

#define MaxN            30000
#define MaxL            16
#define true 1
#define false 0
using namespace std;

static int Init = true;
static char B[2];
static int FullS, NQ;
static int N, M, Half;
static int T[MaxN + 1];
static int D[MaxN + 1];

static void EndQuery(void){
   cout<<"0 Pont\n";
  exit(0);
}

static void Start(void){
  int i;
  B[0] = 0;
  B[1] = 1;
   cin>>N;
  for (i = 1; i <= N; i++){
    T[i] = -1;
    D[i] = 0;
  }
  T[0] = 0;
  D[0] = 0;
  if (N & 1)
    M = N;
  else
    M = N - 1;
  Half = M / 2 + 1;

  Init = false;
  NQ = 0;
  FullS = 0;
  for (i = 0; i < MaxL; i++) {
    if ( ((1 << i) & M) != 0 )
      FullS++;
  }
}

static int Find(int x){
  int Nx, xx;
  Nx = x;
  while (T[Nx] > 0)
    Nx = T[Nx];
  while (x != Nx){
    xx = T[x];
    T[x] = Nx;
    x = xx;
  }
  return Nx;
}

static void Union(int &Nx, int Ny){
  int X;
  if (Nx == 0) {
    Nx = Ny;
    return;
  }
  if (Ny == 0)
    return;
  if (T[Nx] > T[Ny]) {
    X = Nx;
    Nx = Ny;
    Ny = X;
  }
  T[Nx] += T[Ny];
  T[Ny] = Nx;
}

int Size(void){
  if (Init)
    Start();
   cout<< "Size="<<N<<endl;
  return N;
}

int Member(int i, int j){
  int Result, Ri, Rj, Di, Dj, Si, Sj, Ui, Vi, Uj, Vj, SDi, SDj;
  int X;
  int Ans;

  if (Init) {
    cout<<"HIBA: el�bb Size-t kell h�vni!"<<endl;
    EndQuery();
  }

  if (i < 1 || i > N || j < 1 || j > N) {
     cout<<"Member("<<i<<","<<j<<")"<<endl;
      cout<<"HIBA: tartom�nyon k�v�li �rt�k!"<<endl;
    EndQuery();
  }
cout<<"Member("<<i<<","<<j<<")=";
  NQ++;
  Ans = false;
  Ri = Find(i);
  Rj = Find(j);
  Di = D[Ri];
  Dj = D[Rj];
  if (Ri == Rj) {
      cout<<"1\n";
    return 1;
  }
  if (Ri == Dj || Rj == Di) {
     cout<<"0\n";
    return 0;
  }
  Si = abs(T[Ri]);
  Sj = abs(T[Rj]);
  SDi = abs(T[Di]);
  SDj = abs(T[Dj]);
  if (Si >= SDi) {
    Ui = Ri;
    Vi = Di;
  } else {
    Ui = Di;
    Vi = Ri;
    X = Si;
    Si = SDi;
    SDi = X;
  }
  if (Sj >= SDj) {
    Uj = Rj;
    Vj = Dj;
  } else {
    Uj = Dj;
    Vj = Rj;
    X = Sj;
    Sj = SDj;
    SDj = X;
  }
  if (Si + Sj >= Half) {
    Union(Ui, Vj);
    Union(Vi, Uj);
    D[Ui] = Vi;
    if (Vi != 0)
      D[Vi] = Ui;
    Ans = (Find(i)==Find(j));
  } else {
    if (Si + Sj <= SDi + SDj + 2) {
      Union(Ui, Uj);
      Union(Vi, Vj);
      D[Ui] = Vi;
      if (Vi != 0)
	D[Vi] = Ui;
      Ans = (Find(i)==Find(j));
    } else {
      if (Si + SDj == Sj + SDi) {
	X = Si + SDj + Sj + SDi;
	if ((X & M) == X) {
	  Union(Ui, Vj);
	  Union(Vi, Uj);
	  D[Ui] = Vi;
	  if (Vi != 0)
	    D[Vi] = Ui;
	    M &= ~X;
	  Half += -Si - SDj;
          Ans = (Find(i)==Find(j));
	} else {
	  Union(Ui, Uj);
	  Union(Vi, Vj);
	  D[Ui] = Vi;
	  if (Vi != 0)
	    D[Vi] = Ui;
         Ans = (Find(i)==Find(j));
	}
      } else {  /*Si+SDj<>Sj+SDi*/
	Union(Ui, Vj);
	Union(Vi, Uj);
	D[Ui] = Vi;
	if (Vi != 0)
	  D[Vi] = Ui;
       Ans = (Find(i)==Find(j));
      }
    }
  }

  Result = B[Ans];
  cout<<Result<<endl;
  return Result;
}

void Answer(int i){
  int Ri, Di, x, y, Nx, Sum;
  int OK;
  int G[MaxN + 1];
  int a, b, FORLIM;

  if (Init) {
          cout<<"HIBA: el�bb Size-t kell h�vni!"<<endl;
    EndQuery();
  }
  if (i < 1 || i > N) {
     cout<<"Valaszod="<<i<<endl;
      cout<<"HIBA: tarton�nyon k�v�li �rt�k!"<<endl;
      EndQuery();
  }

  Ri = Find(i);
  Di = D[Ri];
  Sum = 0;
  for (x = 1; x <= N; x++) {
    Nx = Find(x);
    G[x] = (Nx != Ri && (T[Nx] < T[D[Nx]] ||
			 T[Nx] == T[D[Nx]] && Nx < D[Nx] || Nx == Di));
  }
  for (x = 1; x <= N; x++) {
    if (T[x] < 0 && x != Di && x != Ri) {
      y = D[x];
      if (T[x] < T[y])
	Sum += abs(T[x]);
      else
	Sum += abs(T[y]);
      T[x] = 0;
      T[y] = 0;
    }
  }
  Sum += abs(T[Di]);
  OK = (Sum <= N / 2);
  cout<<"Valaszod="<<i;
  if (OK) {
      cout<<", Helyes"<<endl;
    } else {
      cout<<", Nem helyes!"<<endl;
  }

  if (OK)
   cout<<"T�bbs�gi csoport:"<<endl;
  else
  cout<<"Kisebbs�gi csoport:"<<endl;
  a = 0;
  b = 0;
  G[0] = true;
  G[N + 1] = true;
  FORLIM = N + 1;
  for (x = 1; x <= FORLIM; x++) {
    if (!G[x]) {
      if (!G[x - 1])
	b = x;
      else {
	a = x;
	b = a;
      }
    } else {
      if (a + 1 < b)
      cout<<a<<".."<<b<<" ";
      else if (a < b)
      cout<<a<<" "<<b<<" ";
      else if (b == a && a != 0)
      cout<<a<<" ";
      b = 0;
    }
  }
  cout<<endl;
  if (OK)
   cout<<"Kisebbs�gi csoport:"<<endl;
  else
     cout<<"T�bbs�gi csoport:"<<endl;
  a = 0;
  b = 0;
  G[0] = false;
  G[N + 1] = false;
  FORLIM = N + 1;
  for (x = 1; x <= FORLIM; x++) {
    if (G[x]) {
      if (G[x - 1])
	b = x;
      else {
	a = x;
	b = a;
      }
    } else {
      if (a + 1 < b)
      cout<<a<<".."<<b<<" ";
      else if (a < b)
      cout<<a<<" "<<b<<" ";
      else if (b == a && a != 0)
      cout<<a<<" ";
      b = 0;
    }
  }
  cout<<endl;
  cout<<"A v�grehajtott k�rd�sek sz�ma: "<<NQ<<endl;

  if (OK) {
    Sum = N - NQ;
    if (Sum < 0)
      Sum = 0;
      cout<<"A lehets�ges maxim�lis pontsz�m: "<<FullS<<endl;
      cout<<"Pontsz�mod: "<<Sum<<endl;
  } else {
    cout<<"A lehets�ges maxim�lis pontsz�m: "<<FullS<<endl;
     cout<<"Pontsz�mod: 0"<<endl;
  }
}
