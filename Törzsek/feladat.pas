Program feladat;

Const
  MaxK=2000;                        { a h�bor�k max. szama  }
  MaxN=1000;						{ a t�rzsek max sz�ma }
Type
	matrix=Array[1..MaxK,1..2] of integer;
	tomb=Array[1..MaxN] of string;
	haboruk=record
                  ki:integer;
                  kivel: integer;
                  mettol:integer;
                  meddig:integer;
                end;
Var
  N		:Integer;	{ a t�rzsek sz�ma }
  K		:Integer;	{h�bor�k sz�ma }
  //T		:tomb;		{ t�rzsek t�mb }
  H 	:array[1..maxK] of haboruk;
  Max	:integer;	{megoldas}

Procedure Beolvas;
  Var i: Integer;
  Begin
    Readln(N);
    Read(K);
    For i:=1 to K do
    begin
		Read(H[i].ki);
		Read(H[i].kivel);
		Read(H[i].mettol);
		Readln(H[i].meddig);
    end;
  End;
  
Procedure KiIr;
  Begin
	Writeln(Max);
  End;
  
Procedure Maximumkivalasztas;
  Var 
	i: integer;
  Begin
	Max:=H[1].meddig-H[1].mettol+1;
	For i:=2 to K do
      If ((H[i].meddig-H[i].mettol+1)>Max) then
		Max:=H[i].meddig-H[i].mettol+1;
  End;

Begin{Program}
 
  Beolvas;

  Maximumkivalasztas;

  KiIr;

End.

