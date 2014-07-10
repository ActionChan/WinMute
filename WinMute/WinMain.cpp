/*
 WinMute
           Copyright (c) 2014 Alexander Steinhoefer

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

#include "StdAfx.h"

HINSTANCE hglobInstance;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
{
   HANDLE hMutex = CreateMutex(nullptr, TRUE, _T("BinaryStudiosWinMute"));
   if (hMutex == NULL) {
      return FALSE;
   }
   if (GetLastError() == ERROR_ALREADY_EXISTS) {
      ReleaseMutex(hMutex);
      MessageBox(0, _T("WinMute is already running."), PROGRAM_NAME,
                 MB_ICONINFORMATION);
      return FALSE;
   }

   hglobInstance = hInstance;

   HeapSetInformation(nullptr, HeapEnableTerminationOnCorruption, nullptr, 0);

   // <Init Common Controls>
   INITCOMMONCONTROLSEX initComCtrl;
   initComCtrl.dwSize = sizeof(INITCOMMONCONTROLSEX);
   initComCtrl.dwICC = ICC_LINK_CLASS;
   if (InitCommonControlsEx(&initComCtrl) == FALSE){
      MessageBox(0, _T("Fatal Error:\n")
                    _T("Failed to register extended window controls"),
                PROGRAM_NAME, MB_ICONSTOP);
      ReleaseMutex(hMutex);
      return FALSE;
   }
   // </Init Common Controls>

   // <Init COM>
   if (CoInitializeEx(0, COINIT_APARTMENTTHREADED) != S_OK) {
      MessageBox(0, _T("Fatal Error:\nFailed to initialize COM library."),
                    PROGRAM_NAME, MB_ICONERROR);
      ReleaseMutex(hMutex);
      return FALSE;
   }

   MSG msg = {0};
   WinMute program;
   if (program.Init()) {
      while (GetMessage(&msg, nullptr, 0, 0)) {
         HWND hwnd = GetForegroundWindow();
         if (!IsWindow(hwnd) || !IsDialogMessage(hwnd, &msg)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
         }
      }
   }

   CoUninitialize();
   ReleaseMutex(hMutex);
   return static_cast<int>(msg.wParam);
}