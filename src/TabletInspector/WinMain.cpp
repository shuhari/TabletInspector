#include "stdafx.h"
#include "MainWindow.h"


CAppModule _Module;


int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR szCmdLine, int nCmdShow) {

    HRESULT hr = CoInitialize(NULL);
    ATLASSERT(SUCCEEDED(hr));
    hr = _Module.Init(NULL, hInstance);
    ATLASSERT(SUCCEEDED(hr));
    BOOL bRet = AtlInitCommonControls(ICC_WIN95_CLASSES);
    ATLASSERT(bRet);

    int nRet;
    {
        CMessageLoop msgLoop;
        bRet = _Module.AddMessageLoop(&msgLoop);
        ATLASSERT(bRet);

        MainWindow mainWin;
        HWND hwndMain = mainWin.CreateEx();
        ATLASSERT(::IsWindow(hwndMain));
        mainWin.ShowWindow(nCmdShow);
        mainWin.UpdateWindow();

        nRet = msgLoop.Run();
        _Module.RemoveMessageLoop();
    }

    _Module.Term();
    CoUninitialize();
    return nRet;
}
