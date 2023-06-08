@rem rem 
@rem rem 
@rem call cxx17 enums.cpp
@rem enums.exe > ..\enums.h
@rem 
@rem if exist enums.exe del enums.exe
@rem if exist enums.obj del enums.obj
@rem 
@rem C:\Program Files\TortoiseGit\bin\TortoiseGitMerge.exe
@rem
@set GEN_FLAGS=--enum-flags=enum-class,type-decl,serialize,deserialize,lowercase


@rem --enum-definition - -F=enumName
@rem --namespace  -N=NS
@rem --enum-name  -E=NAME

@rem EnumNameFormat                          = E$(ENAMNAME)
@rem EnumFlagsNameFormat                     = $(ENAMNAME)Flags
@rem NamespaceNameFormat                     = N$(NS)

@set GEN_FLAGS2=--enum-flags=extra,flags

umba-enum-gen @enums.rsp %GEN_FLAGS% -N=marty_cpp -E=LinefeedType -F=invalid,unknown=-1;detect=0;lf;cr;lfcr;crlf;linefeed_remove ^
%GEN_FLAGS2% --underlaying-type=std::uint32_t --override-template-parameter=EnumFlagsNameFormat:$(ENAMNAME) -E=EnumGeneratorOptionFlagsSerializable -F=@EnumGeneratorOptionFlagsSerializable.txt ^
..\enums.h


