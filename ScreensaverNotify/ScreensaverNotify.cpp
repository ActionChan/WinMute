/*
 ScreensaverNotify DLL
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

#include <SDKDDKVer.h>
#include "ScreensaverNotify.h"


/* =============================================================================
      DLL Globals
   ========================================================================== */

#pragma data_seg(".shared")
static HHOOK hook_ = nullptr;
static HWND hNotifyWnd_ = nullptr;
static UINT notifyWndMsgId_ = 0;
#pragma data_seg()
#pragma comment(linker, "/SECTION:.shared,RWS")

static HINSTANCE hglobInstance_ = nullptr;

/* =============================================================================
      Hook Function
   ========================================================================== */

LRESULT CALLBACK MsgHookProc(UINT code, WPARAM wParam, LPARAM lParam)
{
   if (code >= 0) {
      MSG const * const msg = reinterpret_cast<LPMSG>(lParam);
      if (msg->message == WM_SYSCOMMAND) {
         if (msg->wParam == SC_SCREENSAVE) {
            PostMessage(hNotifyWnd_, notifyWndMsgId_, wParam, lParam);
         }
      }
   }
   return CallNextHookEx(hook_, code, wParam, lParam);
}

/* =============================================================================
      API
   ========================================================================== */

extern "C" {

   WINMUTEDLL_API int RegisterScreensaverHook(HWND hNotifyWnd, UINT notifyWndMsgId)
   {
      if (hook_ != nullptr) {
         UnregisterScreensaverHook();
      }
      hNotifyWnd_ = hNotifyWnd;
      notifyWndMsgId_ = notifyWndMsgId;
      hook_ = SetWindowsHookEx(WH_GETMESSAGE,
                               reinterpret_cast<HOOKPROC>(MsgHookProc),
                               hglobInstance_,
                               0);
      return hook_ != nullptr;
   }

   WINMUTEDLL_API void UnregisterScreensaverHook()
   {
      if (hook_ != nullptr) {
         UnhookWindowsHookEx(hook_);
         hook_ = nullptr;
         hNotifyWnd_ = nullptr;
         notifyWndMsgId_ = 0;
      }
   }

   BOOL APIENTRY DllMain(HINSTANCE hInstance, DWORD reason, LPVOID lpReserved)
   {
      UNREFERENCED_PARAMETER(lpReserved);

      if ((reason == DLL_PROCESS_ATTACH) && (hglobInstance_ == nullptr)) {
         hglobInstance_ = hInstance;
      }

      return TRUE;
   }

} // extern "C"