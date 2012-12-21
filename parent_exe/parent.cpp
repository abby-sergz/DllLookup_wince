
#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <windows.h>

//#define USE_DUMMY_DLL 1

using namespace std;

#define kNAME "parent_"

namespace
{
  void logln(const std::wstring& value)
  {
#ifdef UNDER_CE
    std::wofstream ferr("\\mounted volume\\parent-log.txt", std::ios_base::app);
#else
    std::wofstream ferr("c:\\temp\\parent-log.txt", std::ios_base::app);
#endif

    ferr << L"-----------" << endl;
    ferr << value << endl;
    ferr.flush();
    ferr.close();
  }
}

#ifdef USE_DUMMY_DLL
extern "C" void WINAPI dummy_function(const wchar_t* processName);
#endif


int MyMain()
{
#ifdef USE_DUMMY_DLL
  dummy_function(L"parent");
#endif

  logln(L"Start app");

#ifdef UNDER_CE
  TCHAR* exePath = L"\\Mounted Volume\\dll-lookup\\child\\child_exe.exe";
#else
  TCHAR* exePath = L"child\\child_exe.exe";
#endif
  TCHAR* cmdArgs = {0};

  // create Process
  STARTUPINFO si;
  memset(&si, 0, sizeof(si));
  si.cb = sizeof(STARTUPINFO);
  PROCESS_INFORMATION pi;
  memset(&pi, 0, sizeof(pi));

  BOOL bInstaller = CreateProcess(exePath,
    cmdArgs,
    /* psaProc */NULL,
    /* psaThread */NULL,
    /* InheritHandles */FALSE,
    /* fdwCreate */0,
    /* pvEnv */NULL,
    /* pszCurDir */NULL,
    &si,
    &pi
    );

  if (bInstaller == FALSE)
  {
    std::wstringstream ss;
    ss << L"Can not create process [" << GetLastError() << "]" << endl;
    logln(ss.str());
  }
  else
  {
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
  }
  return 0;
}

#ifdef UNDER_CE
int WinMain(    HINSTANCE hInstance,
            HINSTANCE hPrevInstance,
            LPTSTR lpCmdLine,
            int nShowCmd)
#else
int _tmain(int _Argc, TCHAR* argv[])
#endif
{
  MyMain();
  return 0;
}
