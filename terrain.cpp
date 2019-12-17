#include "terrain.h"
#include "utiles.h"

//D3DVECTOR Position(0,0,0);

Terrain::Terrain():Objet(41*41, 40*40*2*3), delta(5), taille(delta*40/2)
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
			Points[vtx++] = D3DVERTEX( D3DVECTOR( x, y, z), D3DVECTOR( 0, 0, 1 ), tu, tv );
		}
	}
	
	UpdateNormale();
	vtx = 0;
	for(j=0;j<40;j++)
	{
		for(i=0;i<40;i++)
		{
			Indices[vtx++] = j*41+i;
			Indices[vtx++] = j*41+i+1;
			Indices[vtx++] = j*41+i+41;
			Indices[vtx++] = j*41+i+41;
			Indices[vtx++] = j*41+i+1;
			Indices[vtx++] = j*41+i+41+1;
		}
	}

	Texture = dx.CreeTexture("terrain3.bmp");
	if(Texture)
		bOk = true;
	else
		bOk = false;
}
//float infx =-taille, float supx = taille, 
//float infy =-taille, float supy = taille

void Terrain::UpdateNormale()
{

	for(int j=0;j<=40;j++)
	{
		for(int i=0;i<=40;i++)
		{
			D3DVECTOR v = VECTEUR(Points[j*41+i]);
			D3DVECTOR n = Normalize(VECTEUR(Points[j*41+i+41])-v);
			D3DVECTOR s = Normalize(VECTEUR(Points[j*41+i-41])-v);
			D3DVECTOR o = Normalize(VECTEUR(Points[j*41+i-1])-v);
			D3DVECTOR e = Normalize(VECTEUR(Points[j*41+i+1])-v);
			if(i==0)
				o =-e;
			if(i==40)
				e =-o;
			if(j==0)
				s =-n;
			if(j==40)
				n =-s;
			n = D3DVECTOR( n.x,-n.z, n.y);
			s = D3DVECTOR( s.x, s.z,-s.y);
			o = D3DVECTOR( o.z, o.y,-o.x);
			e = D3DVECTOR(-e.z, e.y, e.x);
			v = Normalize(n+s+o+e);
			Points[j*41+i].nx = v.x;
			Points[j*41+i].ny = v.y;
			Points[j*41+i].nz = v.z;
		}
	}
}


float Terrain::Altitude( float x, float y )
{
	int i = Round((x+taille)/delta);
	int j = Round((y+taille)/delta);
	if((i<0)||(i>=41)||(j<0)||(j>=41))
		return 0;
	D3DVECTOR a = VECTEUR(Points[41*j+i]);
	D3DVECTOR b = VECTEUR(Points[41*j+i+1]);
	D3DVECTOR c = VECTEUR(Points[41*(j+1)+i]);
	D3DVECTOR d = VECTEUR(Points[41*(j+1)+i+1]);

	float z1 = a.z + (x-a.x)*(b.z-a.z)/delta;
	float z2 = c.z + (x-c.x)*(d.z-c.z)/delta;
	float z = z1 + (y-a.y)*(z2-z1)/delta;

	return z;
}

void Terrain::Trou( float x, float y , float l)
{
	float z = Altitude(x,y);
	D3DVECTOR v1(x,y,z);
	int minx = (int)((x+taille-l)/delta);
	int miny = (int)((y+taille-l)/delta);
	int maxx = minx + (int)(2*l/delta);
	int maxy = miny + (int)(2*l/delta);
	if(minx<0)
		minx=0;
	if(maxx>40)
		maxx = 40;
	if(miny<0)
		miny=0;
	if(maxy>40)
		maxy = 40;
	int i,j;
	for(j=miny;j<=maxy;j++)
	{
		for(i=minx;i<=maxx;i++)
		{
			D3DVECTOR v2 = VECTEUR(Points[41*j+i])-v1;
			if(SquareMagnitude(v2)<=l*l)
			{
				Points[41*j+i].z = z-Racine(l*l-v2.x*v2.x-v2.y*v2.y);
				if(Points[41*j+i].tu<0.5f)
					Points[41*j+i].tu += 0.66f;
			}
			else if((v2.x*v2.x+v2.y*v2.y<=l*l) && (v2.z>0))
			{
				Points[41*j+i].z -= Racine(l*l-v2.x*v2.x-v2.y*v2.y);
				Points[41*j+i].tu = (Points[41*j+i].z+15)/40;
			}
		}
	}
	UpdateNormale();
	return;
}


D3DVECTOR Terrain::Normal( float x, float y )
{
	int i = Round((x+taille)/delta);
	int j = Round((y+taille)/delta);
	if((i<0)||(i>=41)||(j<0)||(j>=41))
		return D3DVECTOR(0,0,1);
	float ax = Points[41*j+i].x;
	float ay = Points[41*j+i].y;
	D3DVECTOR a = NORMAL(Points[41*j+i]);
	D3DVECTOR b = NORMAL(Points[41*j+i+1]);
	D3DVECTOR c = NORMAL(Points[41*(j+1)+i]);
	D3DVECTOR d = NORMAL(Points[41*(j+1)+i+1]);
	float da = (x-ax)*(y-ay);
	float db = (ax+delta-x)*(y-ay);
	float dc = (x-ax)*(ay+delta-y);
	float dd = (ax+delta-x)*(ay+delta-y);

	return Normalize(dd*a+dc*b+db*c+da*d);
}



