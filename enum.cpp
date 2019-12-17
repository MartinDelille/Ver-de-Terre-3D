#include <windows.h>   /* Standard Windows header file */
#include "directx.h"

/************************************************************************/
/*
 * enumDeviceFunc
 * Callback function which records each usable D3D driver's name and 
 * GUID. It selects the preferred driver on the system by choosing
 * hardware drivers over software drivers, and color lights over 
 * monochrome lights. It sets g_CurrDriver to indicate the 
 * preferred driver.
 */
static HRESULT WINAPI enumDeviceFunc(LPGUID lpGuid, 
									 LPSTR lpDeviceDescription, 
									 LPSTR lpDeviceName,
									 LPD3DDEVICEDESC lpHWDesc, 
									 LPD3DDEVICEDESC lpHELDesc, 
									 LPVOID lpContext)
{
	static bool hardware = false; /* current start driver is software */
	static bool mono = false;	  /* current start driver is color */
	LPD3DDEVICEDESC lpDesc; 	  /* description of current driver */
	
	/* 
	 * Data defined by this application that, when it equals -1, 
	 * indicates that the current driver is the first to be enumerated 
	 */
	bool bStartDriver = (*(int *)lpContext==-1); 

	/*
	 * Decide which device description to consult. The driver is either
	 * hardware (HW) or software. Software drivers provide support
	 * through the hardware emulation layer (HEL).
	 */
	lpDesc = lpHWDesc->dcmColorModel ? lpHWDesc : lpHELDesc;
	/*
	 * If this driver cannot render in the current display bit depth,
	 * return D3DENUMRET_OK to skip this driver. Enumeration continues
	 * automatically as D3D calls enumDeviceFunc again for the next
	 * driver.
	 */

	if (!(lpDesc->dwDeviceRenderBitDepth & g_BPP))
		return D3DENUMRET_OK;
	/*
	 * Record this driver's GUID and name.
	 */
	memcpy(&g_DriverGUID[g_NumDrivers], lpGuid, sizeof(GUID));
	lstrcpy(&g_DriverName[g_NumDrivers][0], lpDeviceName);
	/*
	 * Choose hardware over software, and color lights over monochrome
	 * lights.
	 */
	if (bStartDriver) 
	{
		/*
		 * This is the first valid driver, so record whether this driver
		 * is a hardware driver or not, and whether it is limited to 
		 * monochrome lights or not.
		 */
		g_CurrDriver = g_NumDrivers;
		hardware = lpDesc == lpHWDesc ? true : false;
		mono = lpDesc->dcmColorModel & D3DCOLOR_MONO ? true : false;
	} 
	else if (lpDesc == lpHWDesc && !hardware) 
	{
		/*
		 * If this driver is a hardware driver and the start driver is not,
		 * then make this driver the new preferred driver and record
		 * its hardware and mono capabilities. The next time D3D calls 
		 * enumDeviceFunc, that driver will be compared against this 
		 * new start driver.
		 */
		g_CurrDriver = g_NumDrivers;
		hardware = lpDesc == lpHWDesc ? true : false;
		mono = lpDesc->dcmColorModel & D3DCOLOR_MONO ? true : false;
	} 
	else if ((lpDesc == lpHWDesc && hardware ) || (lpDesc == lpHELDesc
													 && !hardware)) 
	{
		if (lpDesc->dcmColorModel == D3DCOLOR_RGB && mono) 
		{
			/*
			 * If this driver and the start driver are the same type 
			 * (both hardware or both software) and this driver is 
			 * color while the start driver is not, then make this driver
			 * the new preferred driver and record its capabilities.
			 * The next time D3D calls enumDeviceFunc, that driver will 
			 * be compared against this new start driver.
			 */
			g_CurrDriver = g_NumDrivers;
			hardware = lpDesc == lpHWDesc ? true : false;
			mono = lpDesc->dcmColorModel & D3DCOLOR_MONO ? true : false;
		}
	}
	/* 
	 * Increment the current driver number in preparation for the next
	 * driver. 
	 */
	g_NumDrivers++; 

	/* Maximum number of drivers reached, stop enumeration. */
	if (g_NumDrivers == MAX_DRIVERS)
		return (D3DENUMRET_CANCEL);
	/* 
	 * Continue enumerating drivers. D3D will call enumDeviceFunc
	 * again with information for the next driver. 
	 */
	return (D3DENUMRET_OK);
}

