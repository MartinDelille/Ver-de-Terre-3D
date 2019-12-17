#include "objets.h"
#include "utiles.h"
#include "d3dutils.h"

DirectX dx;

Objet::Objet()
{
	Points = NULL;
	NbPoints = 0;
	Indices = NULL;
	NbIndices = 0;
	mat = IdentityMatrix();
	Texture = NULL;
	Couleur = Blanc;
	bOk = false;
}

/*Objet::Objet(int _NbPoints, int _NbIndices) 
	: NbPoints(_NbPoints), NbIndices(_NbIndices)
{
	Points = new D3DVERTEX[NbPoints];
	Indices= new WORD[NbIndices];
	mat = IdentityMatrix();
	Texture = NULL;
	Couleur = Blanc;
	bOk = false;
}*/

Objet::~Objet()
{
	if(bOk)
	{
		delete[] Points;
		delete[] Indices;
	}
}

Objet::Vire()
{
	if(bOk)
		RELEASE(Texture);
	bOk = false;
}

void Objet::Affiche(LPDIRECT3DDEVICE3 pd3dDevice,
                           LPDIRECT3DMATERIAL3 pmtrl)
{
	if(bOk)
	{
		pmtrl->SetMaterial( &mtrl );
		pd3dDevice->SetTexture( 0, Texture );
		
		pd3dDevice->SetTransform(D3DTRANSFORMSTATE_WORLD, &mat );
		if(NbIndices)
			pd3dDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, D3DFVF_VERTEX, 
				Points, NbPoints, Indices, NbIndices, 0 );
		else
			pd3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST, D3DFVF_VERTEX, 
				Points, NbPoints, 0 );
	}
}

void Objet::CalculeNormals()
{
    D3DVECTOR* pNormals = new D3DVECTOR[NbPoints];
    ZeroMemory( pNormals, sizeof(D3DVECTOR)*NbPoints );

    for(int i=0;i<NbIndices;i+=3 )
    {
        WORD a = Indices[i+0];
        WORD b = Indices[i+1];
        WORD c = Indices[i+2];

        D3DVECTOR* v1 = (D3DVECTOR*)&Points[a];
        D3DVECTOR* v2 = (D3DVECTOR*)&Points[b];
        D3DVECTOR* v3 = (D3DVECTOR*)&Points[c];

        D3DVECTOR n = Normalize( CrossProduct( *v2-*v1, *v3-*v2 ) );

        pNormals[a] += n;
        pNormals[b] += n;
        pNormals[c] += n;
    }

    for(i=0;i<NbPoints;i++ )
    {
        pNormals[i] = Normalize( pNormals[i] );

        Points[i].nx = pNormals[i].x;
        Points[i].ny = pNormals[i].y;
        Points[i].nz = pNormals[i].z;
    }

    delete pNormals;
}

Terrain::Terrain(int _N) : n(_N), 
					NbPoints((N+1)*(N+1)), 
					NbIndices((N*N*2*3), 
					delta(5), 
					taille(delta*N/2)
{
	int vtx = 0;
	int i,j;
	float x,y;
	float tu = 0;
	float tv = 0;

	Points = new D3DVERTEX[NbPoints];
	Indices= new WORD[NbIndices];
	mat = IdentityMatrix();
	Texture = NULL;
	Couleur = Blanc;
	bOk = false;

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
	
	vtx = 0;
	for(j=0;j<N;j++)
	{
		for(i=0;i<N;i++)
		{
			Indices[vtx++] = j*(N+1)+i;
			Indices[vtx++] = j*(N+1)+i+1;
			Indices[vtx++] = j*(N+1)+i+(N+1);
			Indices[vtx++] = j*(N+1)+i+(N+1);
			Indices[vtx++] = j*(N+1)+i+1;
			Indices[vtx++] = j*(N+1)+i+(N+1)+1;
		}
	}

	CalculeNormals();
	Texture = dx.CreeTexture("terrain3.bmp");
	if(Texture)
		bOk = true;
	else
		bOk = false;
}

float Terrain::Altitude( float x, float y )
{
	int i = Round((x+taille)/delta);
	int j = Round((y+taille)/delta);
	if((i<0)||(i>=(N+1))||(j<0)||(j>=(N+1)))
		return 0;
	D3DVECTOR a = VECTEUR(Points[(N+1)*j+i]);
	D3DVECTOR b = VECTEUR(Points[(N+1)*j+i+1]);
	D3DVECTOR c = VECTEUR(Points[(N+1)*(j+1)+i]);
	D3DVECTOR d = VECTEUR(Points[(N+1)*(j+1)+i+1]);

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
	if(maxx>N)
		maxx = N;
	if(miny<0)
		miny=0;
	if(maxy>N)
		maxy = N;
	int i,j;
	for(j=miny;j<=maxy;j++)
	{
		for(i=minx;i<=maxx;i++)
		{
			D3DVECTOR v2 = VECTEUR(Points[(N+1)*j+i])-v1;
			if(SquareMagnitude(v2)<=l*l)
			{
				Points[(N+1)*j+i].z = z-Racine(l*l-v2.x*v2.x-v2.y*v2.y);
				if(Points[(N+1)*j+i].tu<0.5f)
					Points[(N+1)*j+i].tu += 0.66f;
			}
			else if((v2.x*v2.x+v2.y*v2.y<=l*l) && (v2.z>0))
			{
				Points[(N+1)*j+i].z -= Racine(l*l-v2.x*v2.x-v2.y*v2.y);
				Points[(N+1)*j+i].tu = (Points[(N+1)*j+i].z+15)/N;
			}
		}
	}
	CalculeNormals();
	return;
}


D3DVECTOR Terrain::Normal( float x, float y )
{
	int i = Round((x+taille)/delta);
	int j = Round((y+taille)/delta);
	if((i<0)||(i>=(N+1))||(j<0)||(j>=(N+1)))
		return D3DVECTOR(0,0,1);
	float ax = Points[(N+1)*j+i].x;
	float ay = Points[(N+1)*j+i].y;
	D3DVECTOR a = NORMAL(Points[(N+1)*j+i]);
	D3DVECTOR b = NORMAL(Points[(N+1)*j+i+1]);
	D3DVECTOR c = NORMAL(Points[(N+1)*(j+1)+i]);
	D3DVECTOR d = NORMAL(Points[(N+1)*(j+1)+i+1]);
	float da = (x-ax)*(y-ay);
	float db = (ax+delta-x)*(y-ay);
	float dc = (x-ax)*(ay+delta-y);
	float dd = (ax+delta-x)*(ay+delta-y);

	return Normalize(dd*a+dc*b+db*c+da*d);
}



Worm::Worm(Terrain* _Sol) : Objet(), gam(0), pos(ZERO), Sol(_Sol)
{
	CreeCylindre(&Points, &NbPoints, &Indices, &NbIndices, 10, 1, 6, 6);
	Points[NbPoints-1].x = 4.2f;
	int i;

	mat = ScaleMatrix(0.6f,0.5f,0.5f)*RotateYMatrix(-pi/4)*TranslateMatrix(-1.0f,0,0);
	for(i=0;i<=6;i++)
		Points[i] *= mat;
	Points[NbPoints-2] *= mat;

	mat = TranslateMatrix(EX/2);
	for(i=7;i<=13;i++)
		Points[i] *= mat;

	mat = TranslateMatrix(EZ);
	for(i=14;i<=20;i++)
		Points[i] *= mat;

	mat = TranslateMatrix(-Points[21].x,0,0)*RotateYMatrix(-pi/20)*TranslateMatrix(0.5f*Points[21].x,0,0);
	for(i=21;i<=27;i++)
		Points[i] *= mat;

	mat = RotateYMatrix(pi/4)*TranslateMatrix(-0.5,0,0);;
	for(i=28;i<=34;i++)
		Points[i] *= mat;

	mat = TranslateMatrix(0.5f,0,0)*RotateYMatrix(pi/2)*TranslateMatrix(1.5f,0,0);
	for(i=35;i<=41;i++)
	{
		Points[i].tv = 0.01f;
		Points[i] *= mat;
	}
	Points[NbPoints-1] *= mat;

	mat = TranslateMatrix(0,0,1.0f);
	for(i=0;i<44;i++)
		Points[i] *= mat;
	mat = ScaleMatrix(0.5f,0.5f,0.5f);
	for(i=0;i<44;i++)
		Points[i] *= mat;

	mat = IdentityMatrix();

	CalculeNormals();
	bOk = false;
}

void Worm::Init()
{
	Texture = dx.CreeTexture("worm.bmp");
	if(Texture)
		bOk = true;
}

void Worm::Bouge()
{
	static float t = 0;	 //  Indicateur du mouvement du ver

	if (dx.Touche(DIK_LEFT))
		gam += pi/32;
	if (dx.Touche(DIK_RIGHT))
		gam -= pi/32;

	D3DVECTOR vDir = RotateZMatrix(gam)*EX;  //Vecteur direction du ver Bill

	if (dx.Touche(DIK_UP))
	{
		pos += 0.4f*vDir;
		t += 0.5f;
	}
	if (dx.Touche(DIK_DOWN))
	{
		pos -= 0.4f*vDir;
		t -= 0.5f;
	}
	pos.z = Sol->Altitude(pos.x,pos.y);

	//				w
	//			   /^\	_   
	//		_  0	|   /| v
	//    \/ \/		| /
	//			    * ___\ u
	//					 /
	//
	//
	D3DVECTOR w = Sol->Normal(pos.x, pos.y);
	D3DVECTOR u = Normalize(D3DVECTOR(vDir.x, vDir.y,-(vDir.x*w.x + vDir.y*w.y)/w.z ));
	D3DVECTOR v = CrossProduct(w, u);

//	D3DMATRIX matRot = PassageMatrix(u, v, w);
	D3DMATRIX matRot = IdentityMatrix();
	matRot(0,0) = u.x;
	matRot(0,1) = u.y;
	matRot(0,2) = u.z;
	matRot(1,0) = v.x;
	matRot(1,1) = v.y;
	matRot(1,2) = v.z;
	matRot(2,0) = w.x;
	matRot(2,1) = w.y;
	matRot(2,2) = w.z;

//	matRot = MatrixInverse(matRot);

	mat = ScaleMatrix(1+Sin(t)/10,1,1)*matRot*TranslateMatrix(pos);
//	mat = RotateZMatrix(gam)*TranslateMatrix(pos);

}

Gun::Gun() : Objet()
{
	CreeCylindre(&Points, &NbPoints, &Indices, &NbIndices, 4, 0.35f, 6, 6);
	Points[NbPoints-2].x = Points[0].x;
	Points[NbPoints-1].x = Points[NbPoints-3].x;

	Texture = dx.CreeTexture("gun.bmp");
	if(Texture)
		bOk = true;
}


Obus::Obus() : Objet(12, 0)
{
	D3DVECTOR v(0,0,0);
	D3DVECTOR v1(1,0,0);
	D3DVECTOR v2(0,1,0);
	D3DVECTOR v3(0,0,1);
	Points[ 0] = D3DVERTEX(  v, v3,0,0);
	Points[ 1] = D3DVERTEX( v2-2*v1, v3,0,0);
	Points[ 2] = D3DVERTEX(-v2-2*v1, v3,0,0);
	Points[ 3] = D3DVERTEX(  v,-v3,0,0);
	Points[ 4] = D3DVERTEX(-v2-2*v1,-v3,0,0);
	Points[ 5] = D3DVERTEX( v2-2*v1,-v3,0,0);
	Points[ 6] = D3DVERTEX(  v, v2,0,0);
	Points[ 7] = D3DVERTEX( v3-2*v1, v2,0,0);
	Points[ 8] = D3DVERTEX(-v3-2*v1, v2,0,0);
	Points[ 9] = D3DVERTEX(  v,-v2,0,0);
	Points[10] = D3DVERTEX(-v3-2*v1,-v2,0,0);
	Points[11] = D3DVERTEX( v3-2*v1,-v2,0,0);

	Couleur = Vert;
	bOk = true;
}

Fleche::Fleche() : Objet()
{
	CreeCylindre(&Points, &NbPoints, &Indices, 
		&NbIndices, 10, 0.5f, 6, 6);

	Points[NbPoints-2].x = Points[0].x;

	int i,j;
	mat = TranslateMatrix(-Points[0].x,0,0);
	for(i=0;i<NbPoints;i++)
		Points[i] *= mat;

	mat = ScaleMatrix(1,2,2)*TranslateMatrix(-1.6f,0,0);
	for(i=NbPoints-9;i<NbPoints-2;i++)
		Points[i] *= mat;

	float s = 1.8f;
	for(j=0;j<NbPoints-9;j+=7)
	{
		mat = ScaleMatrix(1,s,s);
		for(i=j;i<j+7;i++)
			Points[i] *= mat;
		s -= 0.2f;
	}

	Couleur = Rouge;
	mat = IdentityMatrix();
	bOk = true;
}

