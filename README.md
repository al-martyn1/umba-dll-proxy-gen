# Umba-генератор прокси/хук DLL

Данный генератор предназначен для упрощения создания прокси-Dll и хук-DLL.

Прокси-Dll и хук-DLL предназначены для перехвата функций сторонних DLL в исследовательских целях.

**Прокси-DLL** - носит имя оригинальной подменяемой DLL, экспортирует полный набор оригинальных функций
(и данных). Подменяемые функции производят какую-то работу (протоколируют значения параметров etc),
и/или передают управление в соответствующие функции оригинальной DLL, которая располагается рядом 
с прокси или в путях поиска модулей под именем вида `SOME_MODULE_org.dll`. 

Прокси-DLL загружается системным загрузчиком при запуске приложения. Оригинальная DLL (с изменённым именем)
загружается (`LoadLibrary`) по мере необходимости, в момент вызова любой из перехваченных функций.

**Хук-DLL** инжектируется в запущенный процесс (или на этапе запуска процесса). Иницизалия перехватчиков производится
при вызове `DllMain` хук-DLL модуля при помощи функции `GetModuleHandle`, так как использование `LoadLibrary`
запрещено во время работы `DllMain`. Соответственно, перехватываемая DLL уже должна быть загружена в адресное 
пространство процесса.

**Формат вызова**:
>umba-dll-proxy-gen INITIAL_NAMES_INI PROXY_GENERATION_CONFIG_INI PROTOTYPES_LIST_TXT PROXY_FUNCTIONS_LIST_TXT GENERATION_TARGET

Вывод производится в `STDOUT` и может быть перенаправлен в соответствующий файл.

**INITIAL_NAMES_INI** - файл с начальными именами параметров функций в зависимости от типа параметра.
Прототипы функций могут не содержать имён параметров, в то же время для использования в функиях-перехватчиках
все параметры должны быть именованы. При отсутствии имён параметров они назначаются атвтоматически, в соответствии с их типами.

**PROXY_GENERATION_CONFIG_INI** - INI-файл с параметрами генерации.

**PROTOTYPES_LIST_TXT** - файл прототипов перехватываемых функций. Может содержать любое количество прототипов для генерации
нескольких прокси/хук-DLL.

Пустые строки, и строки, содержащие в начале символы "//" или "#" - игнорируются.

Прототипы функций должны быть отдельной строке, перенос прототипа на несколько строк не поддерживается.

Прототипы функций могут быть в форме как собственно прототипов:
>HANDLE CreateFile2(LPCWSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, DWORD dwCreationDisposition, LPCREATEFILE2_EXTENDED_PARAMETERS pCreateExParams);

так и в форме указателя на функцию:
>HANDLE (WINAPI*CreateFile2)(LPCWSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, DWORD dwCreationDisposition, LPCREATEFILE2_EXTENDED_PARAMETERS pCreateExParams);

Вторая форма предпочтительнее, так как позволяет указывать спецификатор вызова (calling conventions) - `__stdcall`, `__cdecl`, `WINAPI` и тп.
В первом случае для корректного разбора необходимо каким-либо образом передавать в парсер прототипов информацию обо всех возможных
спецификаторах вызова, что в данный момент не реализовано.

Имена параметров могут отсутствовать, в качестве параметров могут быть использованы указатели на функции, 
которые, в свою очередь, также могут принимать указатели на функции:
>int  (*sqlite3_create_function16)(sqlite3*,const void*,int,int,void*,void (*sqlite3_xFunc)(sqlite3_context*,int,sqlite3_value**),void (*sqlite3_xStep)(sqlite3_context*,int,sqlite3_value**),void (*sqlite3_xFinal)(sqlite3_context*));


**PROXY_FUNCTIONS_LIST_TXT** - список используемых в модуле-перехватчике функций/данных. Используется формат описания элементов секции
`EXPORT` .DEF-файла:

>entryname[=internal_name|other_module.exported_name] [@ordinal [NONAME] ] [ [PRIVATE] | [DATA] ]

Подробности см. [Rules for module-definition statements/Exports](https://learn.microsoft.com/en-us/cpp/build/reference/exports?view=msvc-170).
Форма с `internal_name` не используется/не протестирована.

Можно использовать forward-форму `other_module.exported_name`. Также поддерживается атрибут `DATA`.


**GENERATION_TARGET** - тип генерируемых выходных данных:

`types` - выводит обнаруженные в возвращаемых значениях, а также в параметрах функций-прототипов типы.
Может использоваться для обновления файла начальных имен параметров INITIAL_NAMES_INI; текстовый отчет.

`config` - файл с define'ами основных опций генерации; текстовый отчет.

`ellipsis` - выводит имена всех функций с переменных числом аргументов (`...` - эллипсис); текстовый отчет.

`data` - выводит все имена, которые являются данными, а не функциями; текстовый отчет.

`forward` - выводит список всех имён, которые системными средствами (forward-механизм DLL) перенаправляются в оригинальную DLL; текстовый отчет.

`proxytypes` - выводит typedef'ы для всех известных прототипов функций:
`typedef HANDLE (WINAPI *CreateFile2_fn_ptr_t)(LPCWSTR, DWORD, DWORD, DWORD, LPCREATEFILE2_EXTENDED_PARAMETERS);`
исходник на C++.

`fntables` - выводит таблицы: указателей на функции и имён функций; исходник на C++.

`fnindexes` - выводит определения (`define`) констант - индексов функций в таблицах; исходник на C++.

`getprocptrfuncs` - реализация функций получения адреса перехватываемых функций; исходник на C++.

`def` - генерирует .DEF файл для создания прокси-DLL; исходник на C++.

`proxycode` - выводит код прокси-функций; исходник на C++.

`hookcode` - выводит код хук-функций; исходник на C++.

`hookinitcode` - инициализация хуков; исходник на C++.

`hookdeinitcode` - деинициализация хуков; исходник на C++.


## Использование

Для генерации прокси-DLL нужен полный список всех экспортируемых оригинальной DLL функций и данных. 
Для генерации хук-DLL это не требуется.

Самый простой способ - использование утилиты `GENDEF` из состава MinGW. Данная утилита генерирует .DEF-файл, 
который можно сразу использовать при создании DLL. Более сложный способ - с использованием утилиты `DUMPBIN` из состава MSVC - тут придётся
поредактировать её вывод. Из .DEF файла извлекается содержимое секции `EXPORTS` (без самого заголовка секции `EXPORTS`)
и помещаем её в файл PROXY_FUNCTIONS_LIST_TXT (для прокси-DLL, для хук-DLL берем только те имена функций, которые будем перехватывать).

Далее потребуется список прототипов функций. Его можно получить разными способами. Например, можно вручную поштучно находить их
в заголовочных файлах. Если повезёт, то можно найти сразу пачкой - так, например, для SQLITE в заголовочных файлах этой библиотеки
есть структура `struct sqlite3_api_routines`, которая содержит указатели на большинство функций SQLITE.
Помещаем прототипы в файл PROTOTYPES_LIST_TXT.

Запускаем `umba-dll-proxy-gen`, при этом INITIAL_NAMES_INI PROXY_GENERATION_CONFIG_INI на данном этапе могут быть пустыми. Задаём генерацию 
отчета о типах параметров функций - types. На основе данного отчета делаем INI-файл с именами параметров, добавляя справа "= paramName" 
для каждого типа:
```
void**                       = ppv
void*                        = pv
int                          = i
const void*                  = pcv
double                       = dbl
char**                       = ppStr
const char*                  = pcStr
LPCWSTR                      = lpcwStr
LPWSTR                       = lpwStr
```
При автоматической генерации имён параметров для одинаковых имен параметров все параметры, кроме первого, нумеруются возрастающим индексом, начиная с 2:
```
void SomeFunc(LPCWSTR lpcwStr, LPCWSTR lpcwStr2, LPCWSTR lpcwStr3);
```

Затем создаём файл INI-файл PROXY_GENERATION_CONFIG_INI с параметрами генерации.
Описание параметров можно посмотреть в исходном файле `src/GenerationOptions.h`.
Пример INI-файла с параметрами генерации (`data\kernel32.ini`):
```
; Also defines MODULENAME:KERNEL32 and modulename:kernel32
SetMacroMulticase  = ModuleName:Kernel32

; Generate 
; #define KERNEL32_HOOK_CONFIG_OPT_NAME 0/1
; for ForwardData/ForwardEllipsis/GenerateCustomHandler options
ConfigDefPrefix                          = $(MODULENAME)_HOOK_CONFIG_

; Generate KERNEL32_HOOK_CONFIG_XXX only for active options (for true values)
ConfigDefOnlyForActiveOption             = true
; ConfigDefOnlyForActiveOption             = false

; Function index in table s macro name format
FunctionIndexConstantNameFormat          = $(MODULENAME)_HOOK_FN_IDX_$(FUNCTIONNAME)

;
EllipsisImplFormat                       = $(MODULENAME)_HOOK_ELLIPSIS_IMPL_$(FUNCTIONNAME)()
ProxyFunctionImplementationPrefix        = $(MODULENAME)_HOOK_EXPORT
ProxyDataFormat                          = $(MODULENAME)_HOOK_EXPORT_DATA $(MODULENAME)_HOOK_DATA_$(DATANAME)()

; Prefix for fnTable$(ModuleName)Pointers and fnTable$(ModuleName)Names
ProxyImplArrayNamesPrefix                = fnTable$(ModuleName)

FunctionPtrTypeFormat                    = $(FunctionName)_fn_ptr_t
GetOriginalFunctionPtrFuncTemplateName   = getOriginal$(ModuleName)FunctionPtr
; Used for hook
getOriginalFunctionFarprocPtrFuncName    = getOriginal$(ModuleName)FunctionFarprocPtr

HookFunctionNameFormat                   = hook_$(FunctionName)

; HookDll/ProxyDll 
Dll                     = $(modulename)_hooks
; Proxy ForwardTarget Dll/Dll with original function for hook
ForwardTarget           = $(modulename)

ForwardData             = true
; ForwardData             = false
ForwardEllipsis         = true
;ForwardEllipsis         = false
; ForwardList             = sqlite3_aggregate_count

ProxyHelo               = $(MODULENAME)_HOOK_TRACE(("!!! Hook called: %s\n", "$(FunctionName)"))
GenerateProxyHelo       = true

; For proxy DLL, use LoadLibrary (UseLoadLibrary=true) to explicit load ForwardTarget DLL
; For hook  DLL, use GetModuleHandle (UseLoadLibrary=false) to get handle of already loaded module
;    In this case ForwardTarget DLL must be one of well known preloaded DLLs, or must be explicitly
;    linked to the hook DLL
UseLoadLibrary          = false

CustomHandlerFormat     = $(MODULENAME)_HOOK_HANDLE_$(FUNCTIONNAME)
GenerateCustomHandler   = true

hookInitStartFormat     = DetourTransactionBegin(); DetourUpdateThread(GetCurrentThread())
hookInitEndFormat       = DetourTransactionCommit()
hookInitFormat          = $(FunctionPointersTable)[$(FunctionIndex)] = $(GetFunctionFarprocPtr)($(FunctionIndex)); DetourAttach(&(PVOID&)$(FunctionPointersTable)[$(FunctionIndex)], (PVOID)$(HookFunctionName))

hookDeinitStartFormat   = DetourTransactionBegin(); DetourUpdateThread(GetCurrentThread())
hookDeinitEndFormat     = DetourTransactionCommit()
hookDeinitFormat        = DetourDetach(&(PVOID&)$(FunctionPointersTable)[$(FunctionIndex)], (PVOID)$(HookFunctionName))
```

После подготовки входных данных последовательно запускаем генерацию C++ файлов. Создаём главный файл DLL-модуля (см. пример `doc/samples/hook_dll_example.cpp`):
```
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


// !!! Add more custom handlers here


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
            // For correct results, DetourRestoreAfterWith should be called in the PROCESS_ATTACH 
            // portion of the DllMain function of the DLL loaded into the target process.
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
```

После произведённых действий получаются исходные тексты готового для компиляции модуля-DLL.

Для того, чтобы добавить своё поведение для перехватчиков, следует определить соответствующие макросы с реализацией кастомного перехвата.

