@rem cls
@echo.
@echo.
@echo.

@if not exist .generated_res mkdir .generated_res
@set GEN=.out\msvc2019\x64\Debug\umba-dll-proxy-gen.exe

@rem set GENERATEDBASE=.generated_res\
@set GENERATEDBASE=


@%GEN% data\initial_names.ini  data\proxy.ini  data\sqlite_prototypes.txt  data\sqlite_proxy_functions_list.txt  types        > %GENERATEDBASE%types.txt
@%GEN% data\initial_names.ini  data\proxy.ini  data\sqlite_prototypes.txt  data\sqlite_proxy_functions_list.txt  ellipsis     > %GENERATEDBASE%ellipsis.txt
@%GEN% data\initial_names.ini  data\proxy.ini  data\sqlite_prototypes.txt  data\sqlite_proxy_functions_list.txt  forward      > %GENERATEDBASE%forward.txt
@%GEN% data\initial_names.ini  data\proxy.ini  data\sqlite_prototypes.txt  data\sqlite_proxy_functions_list.txt  data         > %GENERATEDBASE%data.txt

@%GEN% data\initial_names.ini  data\proxy.ini  data\sqlite_prototypes.txt  data\sqlite_proxy_functions_list.txt  def          > %GENERATEDBASE%e_sqlite3.def

@%GEN% data\initial_names.ini  data\proxy.ini  data\sqlite_prototypes.txt  data\sqlite_proxy_functions_list.txt  config       > %GENERATEDBASE%sqlite_defs_config.h
@%GEN% data\initial_names.ini  data\proxy.ini  data\sqlite_prototypes.txt  data\sqlite_proxy_functions_list.txt  fnindexes    > %GENERATEDBASE%sqlite_fn_idx_defs.h
@%GEN% data\initial_names.ini  data\proxy.ini  data\sqlite_prototypes.txt  data\sqlite_proxy_functions_list.txt  fntables     > %GENERATEDBASE%sqlite_fntables.h
@%GEN% data\initial_names.ini  data\proxy.ini  data\sqlite_prototypes.txt  data\sqlite_proxy_functions_list.txt  proxytypes   > %GENERATEDBASE%sqlite_proxytypes.h
@%GEN% data\initial_names.ini  data\proxy.ini  data\sqlite_prototypes.txt  data\sqlite_proxy_functions_list.txt  proxycode    > %GENERATEDBASE%sqlite_proxybodies.h


@%GEN% data\initial_names.ini  data\proxy.ini  data\sqlite_prototypes.txt  data\sqlite_proxy_functions_list.txt  hookcode       > %GENERATEDBASE%sqlite_hooks.h
@%GEN% data\initial_names.ini  data\proxy.ini  data\sqlite_prototypes.txt  data\sqlite_proxy_functions_list.txt  hookinitcode   > %GENERATEDBASE%sqlite_hooks_init.h
@%GEN% data\initial_names.ini  data\proxy.ini  data\sqlite_prototypes.txt  data\sqlite_proxy_functions_list.txt  hookdeinitcode > %GENERATEDBASE%sqlite_hooks_deinit.h

@if not exist F:\_github\detourse_tests\src\e_sqlite3 mkdir F:\_github\detourse_tests\src\e_sqlite3
@copy /Y %GENERATEDBASE%sqlite_defs_config.h      F:\_github\detourse_tests\src\e_sqlite3\sqlite_defs_config.h
@copy /Y %GENERATEDBASE%sqlite_fn_idx_defs.h      F:\_github\detourse_tests\src\e_sqlite3\sqlite_fn_idx_defs.h
@copy /Y %GENERATEDBASE%sqlite_fntables.h         F:\_github\detourse_tests\src\e_sqlite3\sqlite_fntables.h    
@copy /Y %GENERATEDBASE%sqlite_proxytypes.h       F:\_github\detourse_tests\src\e_sqlite3\sqlite_proxytypes.h 
@copy /Y %GENERATEDBASE%sqlite_proxybodies.h      F:\_github\detourse_tests\src\e_sqlite3\sqlite_proxybodies.h
@copy /Y %GENERATEDBASE%e_sqlite3.def             F:\_github\detourse_tests\src\e_sqlite3\e_sqlite3.def

@if not exist F:\_github\detourse_tests\src\generated_hook mkdir F:\_github\detourse_tests\src\generated_hook
@copy /Y %GENERATEDBASE%sqlite_proxytypes.h       F:\_github\detourse_tests\src\generated_hook\sqlite_proxytypes.h
@copy /Y %GENERATEDBASE%sqlite_fn_idx_defs.h      F:\_github\detourse_tests\src\generated_hook\sqlite_fn_idx_defs.h
@copy /Y %GENERATEDBASE%sqlite_fntables.h         F:\_github\detourse_tests\src\generated_hook\sqlite_fntables.h    
@copy /Y %GENERATEDBASE%sqlite_hooks.h            F:\_github\detourse_tests\src\generated_hook\sqlite_hooks.h       
@copy /Y %GENERATEDBASE%sqlite_hooks_init.h       F:\_github\detourse_tests\src\generated_hook\sqlite_hooks_init.h  
@copy /Y %GENERATEDBASE%sqlite_hooks_deinit.h     F:\_github\detourse_tests\src\generated_hook\sqlite_hooks_deinit.h

