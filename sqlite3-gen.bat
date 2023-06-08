@set GEN=.out\msvc2019\x64\Debug\sqlite3-gen.exe

%GEN% data\sqlite_prototypes.txt data\sqlite_proxy_functions_list.txt proxynumbers > sqlite_fn_idx_defs.h
%GEN% data\sqlite_prototypes.txt data\sqlite_proxy_functions_list.txt fnnametable  > sqlite_fnnames.h
%GEN% data\sqlite_prototypes.txt data\sqlite_proxy_functions_list.txt fnptrtable   > sqlite_fnptrtable.h
%GEN% data\sqlite_prototypes.txt data\sqlite_proxy_functions_list.txt proxytypes   > sqlite_proxytypes.h
%GEN% data\sqlite_prototypes.txt data\sqlite_proxy_functions_list.txt proxybodies  > sqlite_proxybodies.h
