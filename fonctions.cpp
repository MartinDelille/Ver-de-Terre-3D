#define D3D_OVERLOADS

#include <windows.h>
#include <include.h>
#include "directx.h"
#include "fonctions.h"
#include <stdio.h>
#include "d3dutils.h"
#include "utiles.h"
#include "objets.h"
//#include "terrain.h"

DirectX dx;

Terrain Colline(40);
Worm Bill(&Colline);
//Gun *Flingue;
//Fleche *Viseur;
//Obus *Missile;

//bool		bObus = false;
//D3DVECTOR	vObus,dirObus;
//float		ObusAngle;

float zoom = 100;
//bool ModeTir = false;
//bool bTir = false;

LPDIRECT3DLIGHT Lumiere;

bool bFonction = false;

void Statistiques()
{
	static float fFPS	   = 0.0f;
	static float fLastTime = 0.0f;
	static DWORD dwFrames  = 0;

	// Keep track of the time lapse and frame count
	float fTime = timeGetTime() * 0.001f; // Get current time in seconds
	dwFrames++;

	// Update the frame rate once per second
	if( fTime - fLastTime > 1.0f )
	{
		fFPS	  = dwFrames / (fTime - fLastTime);
		fLastTime = fTime;
		dwFrames  = 0;
	}

	// Setup the text buffer to write out
	char buffer[80];
	sprintf( buffer, "%7.02f fps", fFPS);
	dx.Texte( buffer, 0, 0 );
}

bool Dessine()
{
	if(!bFonction)
		return false;
	
	dx.Viewport->Clear2( 1UL, (D3DRECT*)&dx.rcViewport, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, 
		0x00b5b5ff, 1, 0L );

	dx.Device3D->BeginScene();

	Colline.Affiche();
	Bill.Affiche();
	
//	if (ModeTir || bObus)
//		Flingue->Affiche();

//	if (bTir)
//		Viseur->Affiche();

//	if (bObus)
//		Missile->Affiche();

	dx.Device3D->EndScene();

	Statistiques();

	dx.Blit();

	return true;
}

bool Bouge()
{
	if(!bFonction)
		return false;

	if (!dx.InitTouche())
		return false;

	static float alp = pi/8;	//  Angles définissant la
	static float bet = pi/4;	//  position de la caméra
//	static float gam = 0;		//  Angle de direction du ver Bill
//	static float tet = 0;		//  Angle d'inclinaison de l'arme
	static D3DVECTOR b(0,0,0);	//  Vecteur position du ver Bill

	alp += dx.SourisDX/100;
	bet += dx.SourisDY/100;

/*	static bool bEntree = false;
	if (dx.Touche(DIK_RETURN))
	{
		if(!bEntree)
		{
			ModeTir = !ModeTir;
			bObus = false;
			tet = 0;
			bEntree = true;
		}
	}
	else
		bEntree = false;*/

//	D3DMATRIX matFleche;

	/*
	bTir = false;
	if (ModeTir)
	{
		static float force = 0;  //Puissance du tir
		static float t2 = 0;     //Indicateur de temps secondaire
		if (dx.Touche(DIK_UP))
			tet -= pi/32;
		if (dx.Touche(DIK_DOWN))
			tet += pi/32;
		Test(tet,-pi/2,pi/2);
		matFleche = RotateYMatrix(tet)*RotateZMatrix(gam);
		if (dx.Touche(DIK_SPACE))
		{
			bTir = true;
			t2 +=0.1f;
			force = Racine(t2)/2;
		}
		else if(t2!=0)
		{
			ModeTir = false;
			
			dirObus = matFleche*EX;
			dirObus *= force;
			vObus = b;
			t2 = 0;
			ObusAngle = gam;
			bObus = true;
		}
		if(bTir)
			matFleche = ScaleMatrix(force,1,1)*TranslateMatrix(2,0,0)*RotateYMatrix(tet)*TranslateMatrix(1,1,1);
	}
	else
	{
		if (dx.Touche(DIK_UP))
		{
			b += 0.4f*vDir;
			t += 0.5f;
		}
		if (dx.Touche(DIK_DOWN))
		{
			b -= 0.4f*vDir;
			t -= 0.5f;
		}
	}*/

//	Flingue->Bouge(RotateYMatrix(tet)*TranslateMatrix(1,1,1)*matBill);
//	Viseur->Bouge(matFleche*matBill);

/*	static float t3 = 0;
	static bool bExplosion = false;
	if (bObus)
	{
		vObus += dirObus;
		dirObus -= EZ/100;
		float phi = Arctan(dirObus.z/Racine(dirObus.x*dirObus.x+dirObus.y*dirObus.y));
		Missile->Bouge(RotateYMatrix(phi)*RotateZMatrix(ObusAngle)*TranslateMatrix(vObus));
		if(Colline->Altitude(vObus.x, vObus.y)>vObus.z)
		{
			bObus = false;
			bExplosion = true;
			t3 = 10;
			Colline->Trou(vObus.x, vObus.y, 10);
		}
	}

	if(bExplosion)
	{
		t3 -= 0.25f;
		if(t3<=0)
			bExplosion = false;
	}
*/
	Bill->Bouge();
	if(dx.SourisB1)
		zoom -= 5;
	if(dx.SourisB2)
		zoom += 5;
	Test(zoom,10,1000);

	D3DMATRIX matView;
	D3DMATRIX matTemp = RotateYMatrix(bet)*RotateZMatrix(alp);
	static D3DVECTOR vEyePt;
	D3DVECTOR vUpVec;
	D3DVECTOR Position;

/*	if(bObus || bExplosion)
	{
		Position = vObus;
		matView = ViewMatrix( vEyePt+b, Position, EZ );
	}
	else
	{
		vEyePt = zoom*TransformVector(EX, matTemp);
		vUpVec = TransformVector(EZ, matTemp);
		Position = b;
		matView = ViewMatrix( vEyePt+Position, Position, vUpVec );
	}*/

	vEyePt = zoom*TransformVector(EX, matTemp);
	vUpVec = TransformVector(EZ, matTemp);
	Position = b;
	matView = ViewMatrix( vEyePt+Position, Position, vUpVec );

	dx.Device3D->SetTransform(D3DTRANSFORMSTATE_VIEW, &matView );

	return true;
}

bool InitObjets()
{
	HRESULT 		  hr;

	dx.Device3D->SetRenderState( D3DRENDERSTATE_ZENABLE, true );
	dx.Device3D->SetLightState(  D3DLIGHTSTATE_AMBIENT, D3DRGBA( 0.1f, 0.1f, 0.1f, 0.1f));
//	dx.Device3D->SetLightState(  D3DLIGHTSTATE_AMBIENT, D3DRGBA( 1.0f, 1.0f, 1.0f, 1.0f));
//	dx.Device3D->SetRenderState(  D3DRENDERSTATE_SHADEMODE, D3DSHADE_FLAT );

	float fFogEnd = 300.0f;

	dx.Device3D->SetRenderState( D3DRENDERSTATE_FOGENABLE, true );
	dx.Device3D->SetLightState(  D3DLIGHTSTATE_FOGMODE, D3DFOG_LINEAR );
	dx.Device3D->SetLightState(  D3DLIGHTSTATE_FOGEND, *((DWORD *)(&fFogEnd)) );
	dx.Device3D->SetRenderState( D3DRENDERSTATE_FOGCOLOR, 0x00b5b5ff );

	// Set up a texture

	dx.Device3D->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	dx.Device3D->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
	dx.Device3D->SetTextureStageState( 0, D3DTSS_COLOROP,	D3DTOP_MODULATE );
	dx.Device3D->SetTextureStageState( 0, D3DTSS_MINFILTER, D3DTFN_LINEAR );
	dx.Device3D->SetTextureStageState( 0, D3DTSS_MAGFILTER, D3DTFG_LINEAR );
	dx.Device3D->SetTextureStageState( 0, D3DTSS_ADDRESS, D3DTADDRESS_MIRROR );

	// Set the transform matrices
	D3DMATRIX matProj;
	D3DVIEWPORT2 vp;
	vp.dwSize = sizeof(vp);
	dx.Viewport->GetViewport2(&vp);
	float fAspect = ((float)vp.dwHeight) / vp.dwWidth;

	matProj = ProjectionMatrix( 1, 1500, 0.75 );
	dx.Device3D->SetTransform( D3DTRANSFORMSTATE_PROJECTION, &matProj );

	// Set up the lights
	hr = dx.D3D->CreateLight( &Lumiere, NULL );
	if FAILED(hr)
	{
		dx.Erreur("CreateLight", hr);
		return false;
	}

	hr = dx.Viewport->AddLight( Lumiere );
	if FAILED(hr)
	{
		dx.Erreur("AddLight", hr);
		return false;
	}

	D3DLIGHT light;
	ZeroMemory( &light, sizeof(D3DLIGHT) );
	light.dwSize			= sizeof(D3DLIGHT);
	light.dltType			= D3DLIGHT_DIRECTIONAL;//D3DLIGHT_POINT;
	light.dcvColor.r		= 1.0f;
	light.dcvColor.g		= 1.0f;
	light.dcvColor.b		= 1.0f;
	light.dvPosition		= D3DVECTOR( 50, 50, 20 );
	light.dvDirection		= Normalize(D3DVECTOR( 1, 2, -3));
	light.dvAttenuation0	= 0;
	light.dvAttenuation1	= 0;
	light.dvAttenuation2	= 0;
	light.dvRange			= 1;

	hr = Lumiere->SetLight( &light );
	if FAILED(hr)
	{
		dx.Erreur("SetLight", hr);
		return false;
	}

	Colline = new Terrain;
	Bill = new Worm(Colline);
//	Flingue = new Gun;
//	Viseur = new Fleche;
//	Missile = new Obus;

	bFonction = true;

	return true;
}

bool InitApplication(HWND hWnd, HINSTANCE hInstance)
{
	INITBLOC ib;

	ZeroMemory(&ib,sizeof(ib));
	ib.hWnd = hWnd;
	ib.hInst = hInstance;
	ib.bWindowed = false;
	ib.b3D = true;
	ib.bSon = false;
	ib.bInput = true;
	GetWindowRect(hWnd, &ib.rcWindow);

	if (!dx.Init(ib))
		return false;

	if (!InitObjets())
	{
		FermeApplication();
		return false;
	}


	return true;
}

void FermeApplication()
{
	VireObjets();
	RELEASE(Lumiere);

	dx.Vire();
}

void VireObjets()
{
//	delete Bill;
//	delete Colline;
//	delete Flingue;
//	delete Viseur;
//	delete Missile;


	bFonction = false;
}
