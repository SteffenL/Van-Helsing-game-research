// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//  are changed infrequently
//

#pragma once

// Change these values to use different versions
#define WINVER		0x0600
#define _WIN32_WINNT	0x0600
#define _WIN32_IE	0x0700
#define _RICHEDIT_VER	0x0200


#ifndef VC_EXTRALEAN
#	define VC_EXTRALEAN
#endif
/*#ifndef WIN32_LEAN_AND_MEAN
#	define WIN32_LEAN_AND_MEAN
#endif*/

#include <atlbase.h>
#include <atlapp.h>

extern CAppModule _Module;

#define _ATL_NO_AUTOMATIC_NAMESPACE
#define _WTL_NO_AUTOMATIC_NAMESPACE
#define _ATL_USE_DDX_FLOAT
#define _ATL_USE_CSTRING_FLOAT

#include <atlwin.h>
//#include <atlautosizedlg.h>

#include <atlframe.h>
#include <atlctrls.h>
#include <atldlgs.h>
#include <atlddx.h>
#include <atlctrlx.h>
#include <atlmisc.h>
#include <atlcrack.h>
#include <atlctrlw.h>
#include <atltheme.h>
#include <DdxEx.hpp>
#include <CSSFileDialog.hpp>
#include "MyEdit.hpp"

#include <vector>
#include <cassert>
#include <string>
#include <map>

#include "resource.h"

#if defined _M_IX86
  #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
  #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
  #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
  #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
