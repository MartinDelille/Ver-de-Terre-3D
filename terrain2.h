#ifndef _TERRAIN_H_
#define _TERRAIN_H_

#include <include.h>
#include "d3dutils.h"
#include "objets.h"

#define NBP 164


extern int dex;
extern int dey;
extern float delta;
extern float taille;
extern D3DVECTOR Zero,ex,ey,ez;
extern D3DVECTOR Position;

extern D3DVERTEX	Sol[NBP*NBP];
extern int 			SolNbPoints;
extern D3DVERTEX	Sol2[41*41];
extern int 			Sol2NbPoints;
extern WORD			SolIndices[40*40*2*3];
extern int 			SolNbIndices;
extern D3DMATRIX	matTerrain;

void UpdateNormale(float infx, float supx, float infy, float supy);
void AjusteGrille(int inc);
void CreeTerrain();
float Altitude( float x, float y );
void Trou( float x, float y , float l);
D3DVECTOR NormalXY( float x, float y );


