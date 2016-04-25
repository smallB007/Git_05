// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers
#endif

// Modify the following defines if you have to target a platform prior to the ones specified below.
// Refer to MSDN for the latest info on corresponding values for different platforms.
#ifndef WINVER				// Allow use of features specific to Windows XP or later.
#define WINVER 0x0501		// Change this to the appropriate value to target other versions of Windows.
#endif

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows XP or later.                   
#define _WIN32_WINNT 0x0501	// Change this to the appropriate value to target other versions of Windows.
#endif						

#ifndef _WIN32_WINDOWS		// Allow use of features specific to Windows 98 or later.
#define _WIN32_WINDOWS 0x0410 // Change this to the appropriate value to target Windows Me or later.
#endif

#ifndef _WIN32_IE			// Allow use of features specific to IE 6.0 or later.
#define _WIN32_IE 0x0600	// Change this to the appropriate value to target other versions of IE.
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// some CString constructors will be explicit

// turns off MFC's hiding of some common and often safely ignored warning messages
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions


#include <afxdisp.h>        // MFC Automation classes



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT


//std
#include <memory>
#include <vector>
using std::vector;

// BCGPro Control Bar
#include <BCGCBProInc.h>

//libgit2
#include <git2.h>

//POCO
//those two winsock2 headers are required in order for POCO to get no errors during including of POCO headers
#include <winsock2.h>
#include <ws2tcpip.h>
// #include "Poco/Net/HTTPClientSession.h"
// #include "Poco/Net/HTTPSClientSession.h"
// #include "Poco/Net/HTTPRequest.h"
// #include "Poco/Net/HTTPResponse.h"
// #include "Poco/URI.h"
// #include "Poco/Net/NetException.h"


//libpng
//#include <png.h>

//png++
#include <png++/png.hpp>

//Git_05 Utils
#include "Git_05_Utils.h"


#define NO_MFC_DIRECT_2D 
///////////HILO//////////

// Exclude rarely-used stuff from Windows headers
#define WIN32_LEAN_AND_MEAN

// Better type safety for Shell IDLists 
// For more info refer to http://msdn.microsoft.com/en-us/library/bb773321(VS.85).aspx
#define STRICT_TYPED_ITEMIDS

// Don't use min and max macros, we'll use std functions instead
#define NOMINMAX

// Windows Headers
#include <Windows.h>
#include <UIAnimation.h>
#include <commctrl.h>
// Windows Headers
#include <Windows.h>
#include <WinBase.h>
#include <Unknwn.h>
#include <wincodecsdk.h>


// Shell
#include <ShellAPI.h>
#include <ShlObj.h>
#include <StructuredQuery.h>
#include <PropKey.h>

// Graphics
#include <D2d1.h>
#include <DWrite.h>
#include <WinCodec.h>

// Commonly used headers
#include "ComPtr.hpp"
// #include "SharedObject.h"
#include "ComHelpers.hpp"
// 
// #include "AnimationUtility.h"
// #include "ImageEditorInterface.h"
#include "Direct2DUtility.hpp"
// #include "ImageOperation.h"
// #include "Window.h"
// #include "WindowMessageHandler.h"
// #include "WindowFactory.h"

// Useful macros
#ifndef HINST_THISCOMPONENT
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)
#endif

#ifndef IDC_PEN
#define IDC_PEN MAKEINTRESOURCE(32631)
#endif

// Common constants
const double PI = 3.14159265358979323846;

// Use the correct version of the common control library based on the currently selected CPU architecture
// This is needed in order to use TaskDialog, since TaskDialog requires version 6.0 of Comctl32.dll
#if defined _M_IX86
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif

#include <afxcview.h>