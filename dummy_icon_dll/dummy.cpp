
#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>

using namespace std;

#define kNAME "dummy_dll"


namespace
{
  void logln(const std::wstring& value)
  {
#ifdef UNDER_CE
    std::wofstream ferr("\\mounted volume\\dll-log.txt", std::ios_base::app);
#else
    std::wofstream ferr("c:\\temp\\dll-log.txt", std::ios_base::app);
#endif

    ferr << L"-[icon]----" << endl;
    ferr << value << endl;
    ferr.flush();
    ferr.close();
  }
}

extern "C" void WINAPI dummy_function(const wchar_t* processName)
{
  if (NULL != processName)
  {
    logln(processName);
  }
  logln(L"dummy_function");
}


#ifdef UNDER_CE
BOOL APIENTRY DllMain(HANDLE  hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
#else
BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) 
#endif //UNDER_CE
{
  MessageBeep(MB_OK);
  switch (ul_reason_for_call)
  {
    case DLL_PROCESS_ATTACH:
      logln(L"DLL_PROCESS_ATTACH");
      break;
    case DLL_THREAD_ATTACH:
      logln(L"DLL_THREAD_ATTACH");
      break;
    case DLL_THREAD_DETACH:
      logln(L"DLL_THREAD_DETACH");
      break;
    case DLL_PROCESS_DETACH:
      logln(L"DLL_PROCESS_DETACH");
      break;
    default:
      logln(L"DllMain default case");
      break;
  }
  return TRUE;
}
