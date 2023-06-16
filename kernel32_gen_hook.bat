@rem cls
@echo.
@echo.
@echo.

@call gen_config.bat

@%GEN% data\initial_names.ini  data\kernel32.ini  data\kernel32_prototypes.txt  data\kernel32_hook_list.txt  config              > %GENERATEDBASE%kernel32_defs_config.h
@%GEN% data\initial_names.ini  data\kernel32.ini  data\kernel32_prototypes.txt  data\kernel32_hook_list.txt  proxytypes          > %GENERATEDBASE%kernel32_proxytypes.h
@%GEN% data\initial_names.ini  data\kernel32.ini  data\kernel32_prototypes.txt  data\kernel32_hook_list.txt  fnindexes           > %GENERATEDBASE%kernel32_fn_idx_defs.h
@%GEN% data\initial_names.ini  data\kernel32.ini  data\kernel32_prototypes.txt  data\kernel32_hook_list.txt  fntables            > %GENERATEDBASE%kernel32_fntables.h
@%GEN% data\initial_names.ini  data\kernel32.ini  data\kernel32_prototypes.txt  data\kernel32_hook_list.txt  getprocptrfuncs     > %GENERATEDBASE%kernel32_getprocptrfuncs.h
@%GEN% data\initial_names.ini  data\kernel32.ini  data\kernel32_prototypes.txt  data\kernel32_hook_list.txt  hookcode            > %GENERATEDBASE%kernel32_hooks.h
@%GEN% data\initial_names.ini  data\kernel32.ini  data\kernel32_prototypes.txt  data\kernel32_hook_list.txt  hookinitcode        > %GENERATEDBASE%kernel32_hooks_init.h
@%GEN% data\initial_names.ini  data\kernel32.ini  data\kernel32_prototypes.txt  data\kernel32_hook_list.txt  hookdeinitcode      > %GENERATEDBASE%kernel32_hooks_deinit.h


@set TARGET_PATH=F:\_github\detourse_tests\src\generated_hooks_kernel32
@if not exist %TARGET_PATH% mkdir %TARGET_PATH%

@copy /Y %GENERATEDBASE%kernel32_defs_config.h            %TARGET_PATH%\kernel32_defs_config.h
@copy /Y %GENERATEDBASE%kernel32_proxytypes.h             %TARGET_PATH%\kernel32_proxytypes.h
@copy /Y %GENERATEDBASE%kernel32_fn_idx_defs.h            %TARGET_PATH%\kernel32_fn_idx_defs.h
@copy /Y %GENERATEDBASE%kernel32_getprocptrfuncs.h        %TARGET_PATH%\kernel32_getprocptrfuncs.h
@copy /Y %GENERATEDBASE%kernel32_fntables.h               %TARGET_PATH%\kernel32_fntables.h    
@copy /Y %GENERATEDBASE%kernel32_hooks.h                  %TARGET_PATH%\kernel32_hooks.h       
@copy /Y %GENERATEDBASE%kernel32_hooks_init.h             %TARGET_PATH%\kernel32_hooks_init.h  
@copy /Y %GENERATEDBASE%kernel32_hooks_deinit.h           %TARGET_PATH%\kernel32_hooks_deinit.h
