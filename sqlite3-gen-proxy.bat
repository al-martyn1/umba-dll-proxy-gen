@set GEN=.out\msvc2019\x64\Debug\sqlite3-gen.exe


%GEN% proxy.ini  data\sqlite_prototypes.txt data\sqlite_proxy_functions_list.txt ellipsis     > ellipsis.txt
%GEN% proxy.ini  data\sqlite_prototypes.txt data\sqlite_proxy_functions_list.txt def          > e_sqlite3.def
%GEN% proxy.ini  data\sqlite_prototypes.txt data\sqlite_proxy_functions_list.txt fnindexes    > sqlite_fn_idx_defs.h
%GEN% proxy.ini  data\sqlite_prototypes.txt data\sqlite_proxy_functions_list.txt fntables     > sqlite_fntables.h
%GEN% proxy.ini  data\sqlite_prototypes.txt data\sqlite_proxy_functions_list.txt proxytypes   > sqlite_proxytypes.h
%GEN% proxy.ini  data\sqlite_prototypes.txt data\sqlite_proxy_functions_list.txt proxycode    > sqlite_proxybodies.h

copy /Y sqlite_fn_idx_defs.h  F:\_github\detourse_tests\src\e_sqlite3\sqlite_fn_idx_defs.h
copy /Y sqlite_fntables.h     F:\_github\detourse_tests\src\e_sqlite3\sqlite_fntables.h    
copy /Y sqlite_proxytypes.h   F:\_github\detourse_tests\src\e_sqlite3\sqlite_proxytypes.h 
copy /Y sqlite_proxybodies.h  F:\_github\detourse_tests\src\e_sqlite3\sqlite_proxybodies.h
copy /Y e_sqlite3.def         F:\_github\detourse_tests\src\e_sqlite3\e_sqlite3.def
