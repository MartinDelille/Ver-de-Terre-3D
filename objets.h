#ifndef _OBJET_H_
#define _OBJET_H_

//#include <include.h>
//#include <directx.h>
#include "utiles.h"

class Objet : public Point
{
protected:
	D3DVERTEX*	Points;
	int 		NbPoints;
	WORD*		Indices;
	int 		NbIndices;
	D3DMATRIX	mat;
	int			Couleur;
	LPDIRECT3DTEXTURE2 Texture;
	bool bOk;

public:
	Objet();
	~Objet();
	void CalculeNormals();
	void Affiche();
	void Init(){bOk=true;}
	void Vire();
	virtual void Bouge(){}
};

class Terrain : public Objet
{
	float delta;
	float taille;
	int N;
public:
	Terrain();
	void Init();
	float Altitude( float x, float y );
	D3DVECTOR Normal( float x, float y );
	void Trou( float x, float y , float l);
};

class Worm : public Objet
{
	float gam;	//  Angle de direction du ver
	D3DVECTOR pos;	//  Vecteur position du ver Bill
	Terrain* Sol;
public:
	Worm(Terrain* _Sol);
	void Init();
	void Bouge();
};

/*class Gun : public Objet
{
public:
	Gun();
};

class Fleche : public Objet
{
public:
	Fleche();
};

class Obus : public Objet
{
public:
	Obus();
};
*/
//extern DirectX dx;

#endif