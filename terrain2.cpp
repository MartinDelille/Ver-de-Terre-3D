#include "terrain.h"

int dex = 29;
int dey = 12;
float delta = 5;
float taille = delta*(NBP-1)/2;
D3DVECTOR Zero(0,0,0);
D3DVECTOR ex(1,0,0);
D3DVECTOR ey(0,1,0);
D3DVECTOR ez(0,0,1);
D3DVECTOR Position(0,0,0);

D3DVERTEX	Sol[NBP*NBP];
int 		SolNbPoints;
D3DVERTEX	Sol2[41*41];
int 		Sol2NbPoints = 41*41;
WORD		SolIndices[40*40*2*3];
int 		SolNbIndices = 40*40*2*3;
D3DMATRIX	matTerrain = IdentityMatrix();

void UpdateNormale(float infx =-taille, float supx = taille, 
				   float infy =-taille, float supy = taille)
{
	int minx = Round((infx+taille)/delta);
	int maxx = Round((supx+taille)/delta);
	int miny = Round((infy+taille)/delta);
	int maxy = Round((supy+taille)/delta);
	Test(minx,0,NBP-1);
	Test(maxx,0,NBP-1);
	Test(miny,0,NBP-1);
	Test(maxy,0,NBP-1);
	int i,j;

	for(j=miny;j<=maxy;j++)
	{
		for(i=minx;i<=maxx;i++)
		{
			D3DVECTOR v = VECTEUR(Sol[j*NBP+i]);
			D3DVECTOR n = Normalize(VECTEUR(Sol[j*NBP+i+NBP])-v);
			D3DVECTOR s = Normalize(VECTEUR(Sol[j*NBP+i-NBP])-v);
			D3DVECTOR o = Normalize(VECTEUR(Sol[j*NBP+i-1])-v);
			D3DVECTOR e = Normalize(VECTEUR(Sol[j*NBP+i+1])-v);
			if(i==0)
				o =-e;
			if(i==NBP-1)
				e =-o;
			if(j==0)
				s =-n;
			if(j==NBP-1)
				n =-s;
			n = D3DVECTOR( n.x,-n.z, n.y);
			s = D3DVECTOR( s.x, s.z,-s.y);
			o = D3DVECTOR( o.z, o.y,-o.x);
			e = D3DVECTOR(-e.z, e.y, e.x);
			v = Normalize(n+s+o+e);
			Sol[j*NBP+i].nx = v.x;
			Sol[j*NBP+i].ny = v.y;
			Sol[j*NBP+i].nz = v.z;
		}
	}
}

void CreeTerrain()
{
	int vtx = 0;
	int i,j;
	float x,y;
	float tu = 0;
	float tv = 0;

	for(y=-taille;y<=taille;y+=delta)
	{
		tu = 0;
		for(x=-taille;x<=taille;x+=delta)
		{
			float z = 10*Cos(x/20)*Cos(y/20);
			tu = x/(5*delta);
			tv = y/(5*delta);
			Sol[vtx++] = D3DVERTEX( D3DVECTOR( x, y, z), D3DVECTOR( 0, 0, 1 ), tu, tv );
		}
	}
	SolNbPoints =vtx;

	UpdateNormale();
	vtx = 0;
	for(j=0;j<40;j++)
	{
		for(i=0;i<40;i++)
		{
			SolIndices[vtx++] = j*41+i;
			SolIndices[vtx++] = j*41+i+1;
			SolIndices[vtx++] = j*41+i+41;
			SolIndices[vtx++] = j*41+i+41;
			SolIndices[vtx++] = j*41+i+1;
			SolIndices[vtx++] = j*41+i+41+1;
		}
	}
	SolNbIndices = vtx;

	return;
}

void AjusteGrille(int inc)
{
	int px = Round(Position.x/delta) + NBP/2 - dex*inc;
	int py = Round(Position.y/delta) + NBP/2 - dey*inc;
	int i,j;

	if(px<0)
		px = 0;
	if(px>NBP-inc*41)
		px = NBP-inc*41;
	if(py<0)
		py = 0;
	if(py>NBP-inc*41)
		py = NBP-inc*41;

	for(j=0;j<=40;j++)
	{
		for(i=0;i<=40;i++)
			Sol2[j*41+i] = Sol[(py+j*inc)*NBP+px+i*inc];
	}
}

float Altitude( float x, float y )
{
	int i = (int)((x+taille)/delta);
	int j = (int)((y+taille)/delta);
	if((i<0)||(i>=NBP)||(j<0)||(j>=NBP))
		return 0;
	D3DVECTOR a = VECTEUR(Sol[NBP*j+i]);
	D3DVECTOR b = VECTEUR(Sol[NBP*j+i+1]);
	D3DVECTOR c = VECTEUR(Sol[NBP*(j+1)+i]);
	D3DVECTOR d = VECTEUR(Sol[NBP*(j+1)+i+1]);

	float z1 = a.z + (x-a.x)*(b.z-a.z)/delta;
	float z2 = c.z + (x-c.x)*(d.z-c.z)/delta;
	float z = z1 + (y-a.y)*(z2-z1)/delta;

	return z;
}

void Trou( float x, float y , float l)
{
	float z = Altitude(x,y);
	D3DVECTOR v1(x,y,z);
	int minx = (int)((x+taille-l)/delta);
	int miny = (int)((y+taille-l)/delta);
	int maxx = minx + (int)(2*l/delta);
	int maxy = miny + (int)(2*l/delta);
	if(minx<0)
		minx=0;
	if(maxx>=NBP)
		maxx = NBP-1;
	if(miny<0)
		miny=0;
	if(maxy>=NBP)
		maxy = NBP-1;
	int i,j;
	for(j=miny;j<=maxy;j++)
	{
		for(i=minx;i<=maxx;i++)
		{
			D3DVECTOR v2 = VECTEUR(Sol[NBP*j+i])-v1;
			if(SquareMagnitude(v2)<=l*l)
			{
				Sol[NBP*j+i].z = z-Racine(l*l-v2.x*v2.x-v2.y*v2.y);
				if(Sol[NBP*j+i].tu<0.5f)
					Sol[NBP*j+i].tu += 0.66f;
			}
			else if((v2.x*v2.x+v2.y*v2.y<=l*l) && (v2.z>0))
			{
				Sol[NBP*j+i].z -= Racine(l*l-v2.x*v2.x-v2.y*v2.y);
				Sol[NBP*j+i].tu = (Sol[NBP*j+i].z+15)/40;
			}
		}
	}
	UpdateNormale(x-l-2*delta,x+l+2*delta,y-l-2*delta,y+l+2*delta);
	return;
}

D3DVECTOR NormalXY( float x, float y )
{
	int i = (int)((x+taille)/delta);
	int j = (int)((y+taille)/delta);
	if((i<0)||(i>=NBP)||(j<0)||(j>=NBP))
		return D3DVECTOR(0,0,1);
	float ax = Sol[NBP*j+i].x;
	float ay = Sol[NBP*j+i].y;
	D3DVECTOR a = NORMAL(Sol[NBP*j+i]);
	D3DVECTOR b = NORMAL(Sol[NBP*j+i+1]);
	D3DVECTOR c = NORMAL(Sol[NBP*(j+1)+i]);
	D3DVECTOR d = NORMAL(Sol[NBP*(j+1)+i+1]);
	float da = (x-ax)*(y-ay);
	float db = (ax+delta-x)*(y-ay);
	float dc = (x-ax)*(ay+delta-y);
	float dd = (ax+delta-x)*(ay+delta-y);

	return Normalize(dd*a+dc*b+db*c+da*d);
}



