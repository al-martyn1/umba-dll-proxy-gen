#pragma once

#include <exception>
#include <stdexcept>
#include <string>


// failed to deserialize value

namespace marty_cpp {


// Тут нужно int except_id_

// int except_id_base_error                             = 1;
// int except_id_file_not_exist_or_empty                = 1;


class base_error : public std::runtime_error // exception
{

  typedef std::runtime_error base_exception_t;

public:

    /*
    /// returns the explanatory string
    const char* what() const noexcept override
    {
        return m.what();
    }
    */

    const int          id  ;
    const std::string  name;
    const std::string  mtrCategory = "marty_cpp";

    base_error(const std::string& what_arg, int id_ = 0, const std::string &mtrc="marty_cpp") : base_exception_t(what_arg             ), id(id_), name("marty_cpp::base_error"), mtrCategory(mtrc) {}
    base_error(const char* what_arg       , int id_ = 0, const std::string &mtrc="marty_cpp") : base_exception_t(std::string(what_arg)), id(id_), name("marty_cpp::base_error"), mtrCategory(mtrc) {}

    base_error( const char *errName, const char *errMsg, int id_ = 0, const std::string &mtrc="marty_cpp")
    : base_exception_t(std::string("[") + std::string(errName) + std::string("]: ") + std::string(errMsg))
    , id(id_)
    , name(std::string(errName))
    , mtrCategory(mtrc)
    {}

    base_error( const char *errName, const std::string &errMsg, int id_ = 0, const std::string &mtrc="marty_cpp")
    : base_exception_t(std::string("[") + std::string(errName) + std::string("]: ") + errMsg)
    , id(id_)
    , name(std::string(errName))
    , mtrCategory(mtrc)
    {}

    base_error(const base_error &e)
    : base_exception_t(e)
    , id(e.id)
    , name(e.name)
    , mtrCategory(e.mtrCategory)
    {}

}; // class base_error



class file_not_exist_or_empty : public base_error
{

public:

    const std::string fileName;

    file_not_exist_or_empty(const std::string &fileName_, int id_ = 0, const std::string &mtrc="marty_cpp")
    : base_error( "File not exist or empty" // "marty_cpp::file_not_exist_or_empty"
                , fileName_
                , id_
                , mtrc
                )
    , fileName(fileName_)
    {}

    file_not_exist_or_empty(const file_not_exist_or_empty &e)
    : base_error(e)
    , fileName(e.fileName)
    {}

};



class generic_serialization_error : public base_error
{

public:

    const std::string  typeName;
    const std::string  value    ;

    generic_serialization_error( const char *errName, const char *errMsg, const char *typeName_, const std::string &val, int id_ = 0, const std::string &mtrc="marty_cpp" )
    : base_error( errName
                , std::string(errMsg) + std::string(", type: ") + std::string(typeName_) + std::string(", value: '") + val + std::string("'")
                , id_
                , mtrc
                )
    , typeName(typeName_)
    , value(val)
    {}

    generic_serialization_error( const char *errName, const std::string &errMsg, const char *typeName_, const std::string &val, int id_ = 0, const std::string &mtrc="marty_cpp" )
    : base_error( errName
                , errMsg + std::string(", type: ") + std::string(typeName_) + std::string(", value: '") + val + std::string("'")
                , id_
                , mtrc
                )
    , typeName(typeName_)
    , value(val)
    {}

    generic_serialization_error(const generic_serialization_error &e)
    : base_error(e)
    , typeName(e.typeName)
    , value(e.value)
    {}

};




class enum_deserialization_error : public generic_serialization_error
{

public:

    enum_deserialization_error(const char *typeName_, const std::string &val, int id_ = 0, const std::string &mtrc="marty_cpp")
    : generic_serialization_error("Enum/flags deserialization error", "Failed to deserialize value", typeName_, val, id_, mtrc)
    {}

    enum_deserialization_error(const enum_deserialization_error &e)
    : generic_serialization_error(e)
    {}

};


} // namespace marty_cpp

