#pragma once

namespace marty_cpp{

enum class ELinefeedType
{
    invalid          = -1,
    unknown          = -1,
    detect           = 0,
    lf               = 1,
    cr               = 2,
    lfcr             = 3,
    crlf             = 4,
    linefeedRemove   = 5

}; // enum class ELinefeedType

MARTY_CPP_ENUM_CLASS_SERIALIZE_BEGIN( ELinefeedType, std::map, 1 )
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( ELinefeedType::cr              , "Cr"             );
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( ELinefeedType::invalid         , "Invalid"        );
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( ELinefeedType::lfcr            , "Lfcr"           );
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( ELinefeedType::detect          , "Detect"         );
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( ELinefeedType::linefeedRemove  , "LinefeedRemove" );
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( ELinefeedType::lf              , "Lf"             );
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( ELinefeedType::crlf            , "Crlf"           );
MARTY_CPP_ENUM_CLASS_SERIALIZE_END( ELinefeedType, std::map, 1 )

MARTY_CPP_ENUM_CLASS_DESERIALIZE_BEGIN( ELinefeedType, std::map, 1 )
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( ELinefeedType::cr              , "cr"              );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( ELinefeedType::invalid         , "invalid"         );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( ELinefeedType::invalid         , "unknown"         );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( ELinefeedType::lfcr            , "lfcr"            );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( ELinefeedType::detect          , "detect"          );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( ELinefeedType::linefeedRemove  , "linefeed-remove" );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( ELinefeedType::linefeedRemove  , "linefeed_remove" );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( ELinefeedType::linefeedRemove  , "linefeedremove"  );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( ELinefeedType::lf              , "lf"              );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( ELinefeedType::crlf            , "crlf"            );
MARTY_CPP_ENUM_CLASS_DESERIALIZE_END( ELinefeedType, std::map, 1 )



/*!  Соответсвует EnumGeneratorOptionFlags, только по новой технологии */
enum class EnumGeneratorOptionFlagsSerializable : std::uint32_t
{
    invalid                     = (std::uint32_t)(-1),
    unknown                     = (std::uint32_t)(-1),
    enumClass                   = marty_cpp::EnumGeneratorOptionFlags::enumClass,
    unsignedVals                = marty_cpp::EnumGeneratorOptionFlags::unsignedVals,
    umap                        = marty_cpp::EnumGeneratorOptionFlags::useUnorderedMap,
    useUnorderedMap             = marty_cpp::EnumGeneratorOptionFlags::useUnorderedMap,
    singleName                  = marty_cpp::EnumGeneratorOptionFlags::singleDef,
    singleDef                   = marty_cpp::EnumGeneratorOptionFlags::singleDef,
    noExtraLinefeed             = marty_cpp::EnumGeneratorOptionFlags::noExtraLinefeed,
    noxlf                       = marty_cpp::EnumGeneratorOptionFlags::noExtraLinefeed,
    lowercase                   = marty_cpp::EnumGeneratorOptionFlags::lowercaseDeserialize,
    lowercaseDeserialize        = marty_cpp::EnumGeneratorOptionFlags::lowercaseDeserialize,
    integers                    = marty_cpp::EnumGeneratorOptionFlags::integerDeserialize,
    allowIntegers               = marty_cpp::EnumGeneratorOptionFlags::integerDeserialize,
    integerDeserialize          = marty_cpp::EnumGeneratorOptionFlags::integerDeserialize,
    flags                       = marty_cpp::EnumGeneratorOptionFlags::enumFlags,
    enumFlags                   = marty_cpp::EnumGeneratorOptionFlags::enumFlags,
    noComments                  = marty_cpp::EnumGeneratorOptionFlags::disableComments,
    disableComments             = marty_cpp::EnumGeneratorOptionFlags::disableComments,
    serialize                   = marty_cpp::EnumGeneratorOptionFlags::generateDefSerialize,
    genSerialize                = marty_cpp::EnumGeneratorOptionFlags::generateDefSerialize,
    generateDefSerialize        = marty_cpp::EnumGeneratorOptionFlags::generateDefSerialize,
    deserialize                 = marty_cpp::EnumGeneratorOptionFlags::generateDefDeserialize,
    genDeserialize              = marty_cpp::EnumGeneratorOptionFlags::generateDefDeserialize,
    generateDefDeserialize      = marty_cpp::EnumGeneratorOptionFlags::generateDefDeserialize,
    typeDecl                    = marty_cpp::EnumGeneratorOptionFlags::generateDefType,
    typeDeclaration             = marty_cpp::EnumGeneratorOptionFlags::generateDefType,
    genTypeDecl                 = marty_cpp::EnumGeneratorOptionFlags::generateDefType,
    generateDefType             = marty_cpp::EnumGeneratorOptionFlags::generateDefType,
    serializeSet                = marty_cpp::EnumGeneratorOptionFlags::generateDefSerializeSet,
    genSerializeSet             = marty_cpp::EnumGeneratorOptionFlags::generateDefSerializeSet,
    generateDefSerializeSet     = marty_cpp::EnumGeneratorOptionFlags::generateDefSerializeSet,
    deserializeSet              = marty_cpp::EnumGeneratorOptionFlags::generateDefDeserializeSet,
    genDeserializeSet           = marty_cpp::EnumGeneratorOptionFlags::generateDefDeserializeSet,
    generateDefDeserializeSet   = marty_cpp::EnumGeneratorOptionFlags::generateDefDeserializeSet,
    serializeBoth               = marty_cpp::EnumGeneratorOptionFlags::generateDefSerializeBoth,
    genSerializeBoth            = marty_cpp::EnumGeneratorOptionFlags::generateDefSerializeBoth,
    generateDefSerializeBoth    = marty_cpp::EnumGeneratorOptionFlags::generateDefSerializeBoth,
    all                         = marty_cpp::EnumGeneratorOptionFlags::generateDefAll,
    allDefs                     = marty_cpp::EnumGeneratorOptionFlags::generateDefAll,
    genAllDefs                  = marty_cpp::EnumGeneratorOptionFlags::generateDefAll,
    generateDefAll              = marty_cpp::EnumGeneratorOptionFlags::generateDefAll,
    extra                       = marty_cpp::EnumGeneratorOptionFlags::generateDefSerializeExtra,
    generateDefSerializeExtra   = marty_cpp::EnumGeneratorOptionFlags::generateDefSerializeExtra,
    fmtAuto                     = marty_cpp::EnumGeneratorOptionFlags::outputAuto,
    formatAuto                  = marty_cpp::EnumGeneratorOptionFlags::outputAuto,
    outputAuto                  = marty_cpp::EnumGeneratorOptionFlags::outputAuto,
    fmtOct                      = marty_cpp::EnumGeneratorOptionFlags::outputOct,
    formatOct                   = marty_cpp::EnumGeneratorOptionFlags::outputOct,
    outputOct                   = marty_cpp::EnumGeneratorOptionFlags::outputOct,
    fmtDec                      = marty_cpp::EnumGeneratorOptionFlags::outputDec,
    formatDec                   = marty_cpp::EnumGeneratorOptionFlags::outputDec,
    outputDec                   = marty_cpp::EnumGeneratorOptionFlags::outputDec,
    fmtHex                      = marty_cpp::EnumGeneratorOptionFlags::outputHex,
    formatHex                   = marty_cpp::EnumGeneratorOptionFlags::outputHex,
    outputHex                   = marty_cpp::EnumGeneratorOptionFlags::outputHex,
    sys                         = marty_cpp::EnumGeneratorOptionFlags::generateSysIncludesFirst,
    system                      = marty_cpp::EnumGeneratorOptionFlags::generateSysIncludesFirst,
    sysFirst                    = marty_cpp::EnumGeneratorOptionFlags::generateSysIncludesFirst,
    systemFirst                 = marty_cpp::EnumGeneratorOptionFlags::generateSysIncludesFirst

}; // enum class EnumGeneratorOptionFlagsSerializable : std::uint32_t

MARTY_CPP_MAKE_ENUM_FLAGS(EnumGeneratorOptionFlagsSerializable)

MARTY_CPP_ENUM_FLAGS_SERIALIZE_BEGIN( EnumGeneratorOptionFlagsSerializable, std::map, 1 )
    MARTY_CPP_ENUM_FLAGS_SERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::enumClass  , "EnumClass"       );
    MARTY_CPP_ENUM_FLAGS_SERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::invalid    , "Invalid"         );
    MARTY_CPP_ENUM_FLAGS_SERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::integers   , "Integers"        );
    MARTY_CPP_ENUM_FLAGS_SERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::unsignedVals , "UnsignedVals"    );
    MARTY_CPP_ENUM_FLAGS_SERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::noExtraLinefeed , "NoExtraLinefeed" );
    MARTY_CPP_ENUM_FLAGS_SERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::fmtOct     , "FmtOct"          );
    MARTY_CPP_ENUM_FLAGS_SERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::singleName , "SingleName"      );
    MARTY_CPP_ENUM_FLAGS_SERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::lowercase  , "Lowercase"       );
    MARTY_CPP_ENUM_FLAGS_SERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::umap       , "Umap"            );
    MARTY_CPP_ENUM_FLAGS_SERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::flags      , "Flags"           );
    MARTY_CPP_ENUM_FLAGS_SERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::noComments , "NoComments"      );
    MARTY_CPP_ENUM_FLAGS_SERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::serialize  , "Serialize"       );
    MARTY_CPP_ENUM_FLAGS_SERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::deserialize , "Deserialize"     );
    MARTY_CPP_ENUM_FLAGS_SERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::typeDecl   , "TypeDecl"        );
    MARTY_CPP_ENUM_FLAGS_SERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::serializeSet , "SerializeSet"    );
    MARTY_CPP_ENUM_FLAGS_SERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::deserializeSet , "DeserializeSet"  );
    MARTY_CPP_ENUM_FLAGS_SERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::serializeBoth , "SerializeBoth"   );
    MARTY_CPP_ENUM_FLAGS_SERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::all        , "All"             );
    MARTY_CPP_ENUM_FLAGS_SERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::extra      , "Extra"           );
    MARTY_CPP_ENUM_FLAGS_SERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::fmtAuto    , "FmtAuto"         );
    MARTY_CPP_ENUM_FLAGS_SERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::fmtDec     , "FmtDec"          );
    MARTY_CPP_ENUM_FLAGS_SERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::fmtHex     , "FmtHex"          );
    MARTY_CPP_ENUM_FLAGS_SERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::sys        , "Sys"             );
MARTY_CPP_ENUM_FLAGS_SERIALIZE_END( EnumGeneratorOptionFlagsSerializable, std::map, 1 )

MARTY_CPP_ENUM_FLAGS_DESERIALIZE_BEGIN( EnumGeneratorOptionFlagsSerializable, std::map, 1 )
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::enumClass  , "enum-class"                   );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::enumClass  , "enum_class"                   );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::enumClass  , "enumclass"                    );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::invalid    , "invalid"                      );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::invalid    , "unknown"                      );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::integers   , "allow-integers"               );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::integers   , "integers"                     );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::integers   , "allow_integers"               );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::integers   , "integer_deserialize"          );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::integers   , "allowintegers"                );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::integers   , "integer-deserialize"          );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::integers   , "integerdeserialize"           );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::unsignedVals , "unsigned-vals"                );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::unsignedVals , "unsigned_vals"                );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::unsignedVals , "unsignedvals"                 );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::noExtraLinefeed , "no-extra-linefeed"            );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::noExtraLinefeed , "no_extra_linefeed"            );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::noExtraLinefeed , "noextralinefeed"              );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::noExtraLinefeed , "noxlf"                        );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::fmtOct     , "format_oct"                   );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::fmtOct     , "fmt-oct"                      );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::fmtOct     , "format-oct"                   );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::fmtOct     , "fmt_oct"                      );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::fmtOct     , "fmtoct"                       );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::fmtOct     , "formatoct"                    );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::fmtOct     , "output-oct"                   );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::fmtOct     , "output_oct"                   );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::fmtOct     , "outputoct"                    );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::singleName , "single-name"                  );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::singleName , "single_def"                   );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::singleName , "single_name"                  );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::singleName , "singlename"                   );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::singleName , "single-def"                   );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::singleName , "singledef"                    );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::lowercase  , "lowercase_deserialize"        );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::lowercase  , "lowercase"                    );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::lowercase  , "lowercase-deserialize"        );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::lowercase  , "lowercasedeserialize"         );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::umap       , "use-unordered-map"            );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::umap       , "use_unordered_map"            );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::umap       , "useunorderedmap"              );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::umap       , "umap"                         );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::flags      , "enum_flags"                   );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::flags      , "flags"                        );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::flags      , "enum-flags"                   );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::flags      , "enumflags"                    );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::noComments , "no-comments"                  );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::noComments , "no_comments"                  );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::noComments , "disable-comments"             );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::noComments , "nocomments"                   );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::noComments , "disable_comments"             );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::noComments , "disablecomments"              );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::serialize  , "genserialize"                 );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::serialize  , "serialize"                    );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::serialize  , "gen-serialize"                );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::serialize  , "gen_serialize"                );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::serialize  , "generate-def-serialize"       );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::serialize  , "generate_def_serialize"       );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::serialize  , "generatedefserialize"         );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::deserialize , "gendeserialize"               );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::deserialize , "deserialize"                  );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::deserialize , "gen-deserialize"              );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::deserialize , "gen_deserialize"              );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::deserialize , "generate-def-deserialize"     );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::deserialize , "generate_def_deserialize"     );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::deserialize , "generatedefdeserialize"       );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::typeDecl   , "type-declaration"             );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::typeDecl   , "type-decl"                    );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::typeDecl   , "type_declaration"             );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::typeDecl   , "type_decl"                    );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::typeDecl   , "typedecl"                     );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::typeDecl   , "gen-type-decl"                );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::typeDecl   , "typedeclaration"              );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::typeDecl   , "gen_type_decl"                );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::typeDecl   , "gentypedecl"                  );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::typeDecl   , "generate_def_type"            );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::typeDecl   , "generate-def-type"            );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::typeDecl   , "generatedeftype"              );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::serializeSet , "serialize-set"                );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::serializeSet , "serialize_set"                );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::serializeSet , "gen-serialize-set"            );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::serializeSet , "gen_serialize_set"            );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::serializeSet , "genserializeset"              );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::serializeSet , "serializeset"                 );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::serializeSet , "generate-def-serialize-set"   );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::serializeSet , "generate_def_serialize_set"   );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::serializeSet , "generatedefserializeset"      );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::deserializeSet , "deserialize-set"              );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::deserializeSet , "deserialize_set"              );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::deserializeSet , "gendeserializeset"            );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::deserializeSet , "deserializeset"               );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::deserializeSet , "gen_deserialize_set"          );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::deserializeSet , "gen-deserialize-set"          );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::deserializeSet , "generate-def-deserialize-set" );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::deserializeSet , "generate_def_deserialize_set" );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::deserializeSet , "generatedefdeserializeset"    );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::serializeBoth , "serialize-both"               );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::serializeBoth , "serialize_both"               );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::serializeBoth , "gen-serialize-both"           );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::serializeBoth , "gen_serialize_both"           );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::serializeBoth , "genserializeboth"             );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::serializeBoth , "serializeboth"                );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::serializeBoth , "generate-def-serialize-both"  );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::serializeBoth , "generate_def_serialize_both"  );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::serializeBoth , "generatedefserializeboth"     );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::all        , "all"                          );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::all        , "all-defs"                     );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::all        , "all_defs"                     );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::all        , "genalldefs"                   );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::all        , "alldefs"                      );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::all        , "gen-all-defs"                 );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::all        , "generate_def_all"             );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::all        , "gen_all_defs"                 );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::all        , "generate-def-all"             );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::all        , "generatedefall"               );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::extra      , "generatedefserializeextra"    );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::extra      , "extra"                        );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::extra      , "generate-def-serialize-extra" );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::extra      , "generate_def_serialize_extra" );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::fmtAuto    , "format_auto"                  );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::fmtAuto    , "fmt-auto"                     );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::fmtAuto    , "format-auto"                  );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::fmtAuto    , "fmt_auto"                     );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::fmtAuto    , "fmtauto"                      );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::fmtAuto    , "formatauto"                   );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::fmtAuto    , "output-auto"                  );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::fmtAuto    , "output_auto"                  );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::fmtAuto    , "outputauto"                   );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::fmtDec     , "format_dec"                   );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::fmtDec     , "fmt-dec"                      );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::fmtDec     , "format-dec"                   );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::fmtDec     , "fmt_dec"                      );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::fmtDec     , "fmtdec"                       );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::fmtDec     , "formatdec"                    );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::fmtDec     , "output-dec"                   );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::fmtDec     , "output_dec"                   );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::fmtDec     , "outputdec"                    );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::fmtHex     , "format_hex"                   );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::fmtHex     , "fmt-hex"                      );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::fmtHex     , "format-hex"                   );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::fmtHex     , "fmt_hex"                      );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::fmtHex     , "fmthex"                       );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::fmtHex     , "formathex"                    );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::fmtHex     , "output-hex"                   );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::fmtHex     , "output_hex"                   );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::fmtHex     , "outputhex"                    );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::sys        , "sys"                          );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::sys        , "sysfirst"                     );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::sys        , "system"                       );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::sys        , "system_first"                 );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::sys        , "sys-first"                    );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::sys        , "system-first"                 );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::sys        , "sys_first"                    );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumGeneratorOptionFlagsSerializable::sys        , "systemfirst"                  );
MARTY_CPP_ENUM_FLAGS_DESERIALIZE_END( EnumGeneratorOptionFlagsSerializable, std::map, 1 )

MARTY_CPP_ENUM_FLAGS_SERIALIZE_SET(EnumGeneratorOptionFlagsSerializable, std::set)

MARTY_CPP_ENUM_FLAGS_DESERIALIZE_SET(EnumGeneratorOptionFlagsSerializable, std::set)

} // namespace marty_cpp

