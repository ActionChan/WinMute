/*
 WinMute
           Copyright (c) 2017, Alexander Steinhoefer

-----------------------------------------------------------------------------
Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice,
      this list of conditions and the following disclaimer.

    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.

    * Neither the name of the author nor the names of its contributors may
      be used to endorse or promote products derived from this software
      without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.
-----------------------------------------------------------------------------
*/

#pragma once

#ifdef _UNICODE
   #if defined _M_IX86
      #pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
   #elif defined _M_IA64
      #pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
   #elif defined _M_X64
      #pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
   #else
      #pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
   #endif
#endif

#define STRICT
#define _WIN32_WINNT 0x0600
#include <sdkddkver.h>

#include <cassert>
#include <string>
#include <memory>
#include <vector>
#include <algorithm>

#include <windows.h>
#include <commctrl.h>
#include <WtsApi32.h>
#include <tchar.h>
#include <strsafe.h>
#include <time.h>

#include "resource.h"

#include "ScreensaverNotifier.h"
#include "Settings.h"
#include "TrayIcon.h"
#include "WinMute.h"
#include "VersionHelper.h"

template<class Interface>
inline void SafeRelease(Interface** ppInterfaceToRelease)
{
   if (*ppInterfaceToRelease) {
      (*ppInterfaceToRelease)->Release();
      (*ppInterfaceToRelease) = nullptr;
   }
}

#ifdef UNICODE
   typedef std::basic_string<wchar_t> tstring;
#else
   typedef std::basic_string<char> tstring;
#endif

// Utility
void PrintWindowsError(LPTSTR lpszFunction, DWORD lastError = -1);

static const LPCTSTR PROGRAM_NAME = _T("WinMute");
