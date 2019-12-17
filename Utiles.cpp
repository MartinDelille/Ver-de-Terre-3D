#include "utiles.h"
#include <d3dutils.h>
#include <math.h>

const float Pi = 3.14159265358f;

D3DVECTOR operator*(D3DMATRIX& mat, D3DVECTOR& v)
{
	float	hvec[4];

	for (int i=0; i<4; i++)
	{
		hvec[i] = 0.0f;
		for (int j=0; j<4; j++)
		{
			if (j==3) 
				hvec[i] += mat(j, i);
			else 
				hvec[i] += v[j] * mat(j, i);
		}
	}
	D3DVECTOR ret(hvec[0]/hvec[3], hvec[1]/hvec[3], hvec[2]/hvec[3]);

	return ret;
}

D3DVECTOR& operator*=(D3DVECTOR& v, D3DMATRIX& mat)
{
	v = mat*v;
	return v;
}

D3DVERTEX operator*(D3DMATRIX& mat, D3DVERTEX& vertex)
{
	D3DVECTOR v = mat*VECTEUR(vertex);
	D3DVECTOR n = TransformNormalPureRotation(NORMAL(vertex), mat);
	float tu = vertex.tu;
	float tv = vertex.tv;
	return D3DVERTEX(v, n, tu, tv);

}

D3DVERTEX& operator*=(D3DVERTEX& vertex, D3DMATRIX& mat)
{
	vertex = mat*vertex;
	return vertex;
}

D3DMATRIX
PassageMatrix(D3DVECTOR& v1, D3DVECTOR& v2, D3DVECTOR& v3)
{
	D3DMATRIX ret = IdentityMatrix();

	ret(0,0) = v1.x;
	ret(0,1) = v1.y;
	ret(0,2) = v1.z;

	ret(1,0) = v2.x;
	ret(1,1) = v2.y;
	ret(1,2) = v2.z;;

	ret(2,0) = v3.x;
	ret(2,1) = v3.y;
	ret(2,2) = v3.z;;

	return ret;
} 

void CreeCylindre( D3DVERTEX** pPoints, int* pNbPoints,
					  WORD** pIndices, int* pNbIndices,
					  float Longueur, float Rayon, int NbSpires, int NbAnneaux )
{
	// Allocate memory for the vertices and indices
	int NbPoints	= (NbAnneaux+1)*NbSpires+2;
	int NbIndices	= 6*NbAnneaux*(NbSpires-1)+2*3*NbAnneaux;
	D3DVERTEX* Points = new D3DVERTEX[NbPoints];
	WORD* Indices = new WORD[NbIndices];

	int x, y, vtx = 0, index = 0;

	float alp = (float)2*pi/NbAnneaux;
	float inc = Longueur/(NbSpires+1);

	float x0 = -Longueur/2 + inc;
	for( y=0; y<=NbAnneaux; y++ )
	{
		D3DVECTOR v(x0,Rayon*Sin(y*alp),Rayon*Cos(y*alp));
		D3DVECTOR n(-0.5f,Sin(y*alp),Cos(y*alp));
		float tu = y*alp/(2*pi);
		float tv = 0.5f-x0/Longueur;
		Points[vtx++] = D3DVERTEX( v, Normalize(n), tu, tv );
	}
	for( x=1; x<NbSpires-1; x++ )
	{
		x0 += inc;
		for( y=0; y<=NbAnneaux; y++ )
		{
			D3DVECTOR v(x0,Rayon*Sin(y*alp),Rayon*Cos(y*alp));
			D3DVECTOR n(0,Sin(y*alp),Cos(y*alp));
			float tu = y*alp/(2*pi);
			float tv = 0.5f-x0/Longueur;
			Points[vtx++] = D3DVERTEX( v, n, tu, tv );
		}
	}
	x0 += inc;
	for( y=0; y<=NbAnneaux; y++ )
	{
		D3DVECTOR v(x0,Rayon*Sin(y*alp),Rayon*Cos(y*alp));
		D3DVECTOR n(0.5f, Sin(y*alp), Cos(y*alp));
		float tu = y*alp/(2*pi);
		float tv = 0.5f-x0/Longueur;
		Points[vtx++] = D3DVERTEX( v, Normalize(n), tu, tv );
	}

	for( x=0; x<NbSpires-1; x++ )
	{
		for( y=0; y<NbAnneaux; y++ )
		{
			Indices[index++] = (WORD)( (x+0)*(NbAnneaux+1)+y);
			Indices[index++] = (WORD)( (x+1)*(NbAnneaux+1)+y);
			Indices[index++] = (WORD)( (x+0)*(NbAnneaux+1)+y+1);
			Indices[index++] = (WORD)( (x+0)*(NbAnneaux+1)+y+1); 
			Indices[index++] = (WORD)( (x+1)*(NbAnneaux+1)+y);
			Indices[index++] = (WORD)( (x+1)*(NbAnneaux+1)+y+1);
		}
	}
	// Make top and bottom
	WORD wNorthVtx = vtx;
	Points[vtx++] = D3DVERTEX(-Longueur/2*EX,-EX, 0.5f, 1 );
	WORD wSouthVtx = vtx;
	Points[vtx++] = D3DVERTEX( Longueur/2*EX, EX, 0.5f, 0 );

	for( x=0; x<NbAnneaux; x++ )
	{
		Indices[index++] = wNorthVtx;
		Indices[index++] = x;
		Indices[index++] = x+1;
	}

	for( x=0; x<NbAnneaux; x++ )
	{
		Indices[index++] = wSouthVtx;
		Indices[index++] = (WORD)((NbSpires-1)*(NbAnneaux+1)+x+1);
		Indices[index++] = (WORD)((NbSpires-1)*(NbAnneaux+1)+x);
	}

	*pNbPoints = NbPoints;
	*pNbIndices = NbIndices;
	*pPoints = Points;
	*pIndices = Indices;

	return;
}

void InitMaterial( D3DMATERIAL &mtrl, float r, float g, float b )
{
	ZeroMemory( &mtrl, sizeof(D3DMATERIAL) );
	mtrl.dwSize 	  = sizeof(D3DMATERIAL);
	mtrl.dcvDiffuse.r = mtrl.dcvAmbient.r = r;
	mtrl.dcvDiffuse.g = mtrl.dcvAmbient.g = g;
	mtrl.dcvDiffuse.b = mtrl.dcvAmbient.b = b;
	mtrl.dwRampSize   = 16L; // A default ramp size
	mtrl.power = 40;
}

