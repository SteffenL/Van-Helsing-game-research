#include "stdafx.h"

#include "resource.h"

#include "aboutdlg.h"
#include "MainDlg.h"

#include "my_console_output_buffer.h"
#include <common/Log.h>

CAppModule _Module;

int Run(LPTSTR /*lpstrCmdLine*/ = NULL, int nCmdShow = SW_SHOWDEFAULT)
{
    // For logging; redirects output so that it works with the allocated console
    if (::AllocConsole()) {
        ::freopen("CONOUT$", "w", stdout);
        auto h = ::GetStdHandle(STD_OUTPUT_HANDLE);
        // For nowide; output is not displayed otherwise
        nowide::cout.set_rdbuf(new nowide::details::my_console_output_buffer(h));
    }

    bool shouldBeVerbose = true;
    if (shouldBeVerbose) {
        using namespace vanhelsing::engine;
        Log::SetLogLevelFilter(LogLevel::Trace);
    }

	CMessageLoop theLoop;
	_Module.AddMessageLoop(&theLoop);

	CMainDlg dlgMain;

	if(dlgMain.Create(NULL) == NULL)
	{
		ATLTRACE(_T("Main dialog creation failed!\n"));
		return 0;
	}

	dlgMain.ShowWindow(nCmdShow);

	int nRet = theLoop.Run();

	_Module.RemoveMessageLoop();
	return nRet;
}

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR lpstrCmdLine, int nCmdShow)
{
	HRESULT hRes = ::CoInitializeEx(NULL, COINIT_MULTITHREADED);
	ATLASSERT(SUCCEEDED(hRes));

	// this resolves ATL window thunking problem when Microsoft Layer for Unicode (MSLU) is used
	::DefWindowProc(NULL, 0, 0, 0L);

	AtlInitCommonControls(ICC_BAR_CLASSES);	// add flags to support other controls

	hRes = _Module.Init(NULL, hInstance);
	ATLASSERT(SUCCEEDED(hRes));

	int nRet = Run(lpstrCmdLine, nCmdShow);

	_Module.Term();
	::CoUninitialize();

	return nRet;
}
