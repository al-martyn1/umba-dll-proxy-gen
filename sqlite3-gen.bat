@set GEN=.out\msvc2019\x64\Debug\sqlite3-gen.exe


%GEN% data\sqlite_prototypes.txt data\sqlite_proxy_functions_list.txt ellipsis     > ellipsis.txt
%GEN% data\sqlite_prototypes.txt data\sqlite_proxy_functions_list.txt proxynumbers > sqlite_fn_idx_defs.h
%GEN% data\sqlite_prototypes.txt data\sqlite_proxy_functions_list.txt fntables     > sqlite_fntables.h
%GEN% data\sqlite_prototypes.txt data\sqlite_proxy_functions_list.txt proxytypes   > sqlite_proxytypes.h
%GEN% data\sqlite_prototypes.txt data\sqlite_proxy_functions_list.txt proxybodies  > sqlite_proxybodies.h

copy /Y sqlite_fn_idx_defs.h F:\_github\detourse_tests\src\e_sqlite3\sqlite_fn_idx_defs.h
copy /Y sqlite_fntables.h     F:\_github\detourse_tests\src\e_sqlite3\sqlite_fntables.h    
copy /Y sqlite_proxytypes.h  F:\_github\detourse_tests\src\e_sqlite3\sqlite_proxytypes.h 
copy /Y sqlite_proxybodies.h F:\_github\detourse_tests\src\e_sqlite3\sqlite_proxybodies.h
