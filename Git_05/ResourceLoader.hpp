#pragma once
#include "windows.h"
class ResourceLoader
{
public:
	ResourceLoader();
   ~ResourceLoader();
   HRESULT LoadResourceBitmap(ID2D1RenderTarget *pRenderTarget,
	   IWICImagingFactory *pIWICFactory, 
	   PCWSTR resourceName, 
	   PCWSTR resourceType,
	   UINT destinationWidth,
	   UINT destinationHeight, 
	   ID2D1Bitmap **ppBitmap);
};

