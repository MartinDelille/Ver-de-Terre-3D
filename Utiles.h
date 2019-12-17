#ifndef _UTILES_H_
#define _UTILES_H_

#define D3D_OVERLOADS
#include <include.h>
#include <math.h>

extern const float Pi;
#define VECTEUR(v) D3DVECTOR(v.x,v.y,v.z)
#define NORMAL(v) D3DVECTOR(v.nx,v.ny,v.nz)

#define Cos(x) (float)cos(x)
#define Sin(x) (float)sin(x)
#define Arctan(x) (float)atan(x)
#define Racine(x) (float)sqrt(x)
#define Abs(x) (float)fabs(x)
#define Round(x) (int)floor(x)
#define Test(x,min,max) {if(x<min) x=min;if(x>max) x=max;}

#define ZERO D3DVECTOR(0,0,0)
#define EX D3DVECTOR(1,0,0)
#define EY D3DVECTOR(0,1,0)
#define EZ D3DVECTOR(0,0,1)

class Point
{
protected:
	D3DVECTOR pos;
public:
	Point(){pos=ZERO;}
	Point(D3DVECTOR _pos){pos=_pos;}
}

class Camera : public Point
{

}

D3DVECTOR operator*(D3DMATRIX& mat, D3DVECTOR& v);
D3DVECTOR& operator*=(D3DVECTOR& v, D3DMATRIX& mat);
D3DVERTEX operator*(D3DMATRIX& mat, D3DVERTEX& vertex);
D3DVERTEX& operator*=(D3DVERTEX& vertex, D3DMATRIX& mat);

D3DMATRIX PassageMatrix(D3DVECTOR& v1, D3DVECTOR& v2, D3DVECTOR& v3);

void CreeCylindre( D3DVERTEX** Points, int* NbPoints,
					  WORD**Indices, int* NbIndices,
					  float Longueur, float Rayon, 
					  int NbSpires, int NbAnneaux );

void InitMaterial( D3DMATERIAL &mtrl, float r=0.0f, float g=0.0f, float b=0.0f );

#endif
