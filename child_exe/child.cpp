#include "stdafx.h"
#include <iostream>
#include <string>
#include <fstream>
#include <windows.h>
#ifdef UNDER_CE
#include <tlhelp32.h>
#else
#include <psapi.h>
#endif

using namespace std;

#define kNAME "child_"

namespace
{
  void logln(const std::wstring& value)
  {
#ifdef UNDER_CE
    std::wofstream ferr("\\mounted volume\\child-log.txt", std::ios_base::app);
#else
    std::wofstream ferr("c:\\temp\\child-log.txt", std::ios_base::app);
#endif

    ferr << L"-----------" << endl;
    ferr << value << endl;
    ferr.flush();
    ferr.close();
  }
}

namespace
{
  void PrintModules(DWORD processID)
  {
    HANDLE hProcess = OpenProcess( PROCESS_QUERY_INFORMATION |
                                   PROCESS_VM_READ,
                                   FALSE, // inherit handle
                                   processID );

    if (NULL != hProcess)
    {
#ifdef UNDER_CE
      HANDLE snapShot = CreateToolhelp32Snapshot(
                                                TH32CS_GETALLMODS | TH32CS_SNAPMODULE,
                                                processID
                                              );

      if (INVALID_HANDLE_VALUE != snapShot)
      {
        MODULEENTRY32 module = {0};
        module.dwSize = sizeof(module);
        if (FALSE != Module32First(snapShot, &module))
        {
          do
          {
            logln(module.szModule);
            logln(module.szExePath);
          } while(FALSE != Module32Next(snapShot, &module));
        }
        else
        {
          logln(L"can not start module enumeration");
        }
        CloseToolhelp32Snapshot(snapShot);
      }
      else
      {
        logln(L"can not make snapshot");
      }
#else
      HMODULE hMods[1024];
      DWORD cbNeeded;
      if( EnumProcessModules(hProcess, hMods, sizeof(hMods), &cbNeeded))
      {
          unsigned int i;
          for ( i = 0; i < (cbNeeded / sizeof(HMODULE)); i++ )
          {
              TCHAR szModName[MAX_PATH];

              // Get the full path to the module's file.

              if ( GetModuleFileNameEx( hProcess, hMods[i], szModName,
                                        sizeof(szModName) / sizeof(TCHAR)))
              {
                  // Print the module name and handle value.

                logln(szModName);
              }
          }
      }
#endif
      CloseHandle( hProcess );
    }

  }
}

extern "C" void WINAPI dummy_function(const wchar_t* processName);

int MyMain()
{
  dummy_function(L"child");
  ::PrintModules(GetCurrentProcessId());

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
