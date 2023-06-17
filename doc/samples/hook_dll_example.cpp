/*! \file
    \brief DLL для инжекта, делаем перехват CreateFile2, и всякого из kernel32
 */

#define DETOUR_DEBUG

#include <winsock2.h>
#include <windows.h>
#include <string>
#include <vector>
#include <../_3dp/Detours/src/detours.h>

#pragma comment( lib, "detours.lib" )


#include "generated_hooks_kernel32\kernel32_fn_idx_defs.h"
#include "generated_hooks_kernel32\kernel32_fntables.h"
#include "generated_hooks_kernel32\kernel32_proxytypes.h"

#include "generated_hooks_kernel32\kernel32_getprocptrfuncs.h"


#define KERNEL32_HOOK_TRACE(x) DETOUR_TRACE(x)

#define KERNEL32_HOOK_EXPORT
#define KERNEL32_HOOK_EXPORT_DATA


#define KERNEL32_HOOK_HANDLE_CREATEFILE2()  \
                                            \
            do                              \
            {                               \
                KERNEL32_HOOK_TRACE(("CreateFile2(%s)\n", to_ascii(lpFileName).c_str())); \
                                            \
            } while(0)


// !!! Add custom handlers here


#include "generated_hooks_kernel32\kernel32_hooks.h"


void init_hook(HINSTANCE hinstDLL)
{
    DETOUR_TRACE(("Hello debugger from init_hook!\n"));

    #include "generated_hooks_kernel32\kernel32_hooks_init.h"
}

void deinit_hook(HINSTANCE hinstDLL)
{
    DETOUR_TRACE(("Hello debugger from deinit_hook!\n"));

    #include "generated_hooks_kernel32\kernel32_hooks_deinit.h"
}


BOOL WINAPI DllMain(
    HINSTANCE hinstDLL,  // handle to DLL module
    DWORD fdwReason,     // reason for calling function
    LPVOID lpvReserved )  // reserved
{
    if (DetourIsHelperProcess())
    {
        return TRUE;
    }

    switch( fdwReason ) 
    { 
        case DLL_PROCESS_ATTACH:
         // Initialize once for each new process.
         // Return FALSE to fail DLL load.
            // For correct results, DetourRestoreAfterWith should be called in the PROCESS_ATTACH portion of the DllMain function of the DLL loaded into the target process.
            DetourRestoreAfterWith();
            init_hook(hinstDLL);

            break;

        case DLL_THREAD_ATTACH:
         // Do thread-specific initialization.
            break;

        case DLL_THREAD_DETACH:
         // Do thread-specific cleanup.
            break;

        case DLL_PROCESS_DETACH:
        
            if (lpvReserved != nullptr)
            {
                break; // do not do cleanup if process termination scenario
            }
            
         // Perform any necessary cleanup.

            deinit_hook(hinstDLL);

            break;
    }
    return TRUE;  // Successful DLL_PROCESS_ATTACH.}
}


