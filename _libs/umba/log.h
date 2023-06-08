/*! \file
    \brief Хелперы для утилит командной строки (CLI)
*/


#if !defined(UMBA_SIMPLE_FORMATTER_H)
    #error "umba/cli_log.h requires umba/simple_formatter.h to be included first"
#endif

#include <string>
#include <unordered_map>
    

// requires global vars 
extern bool umbaLogGccFormat  ;
extern bool umbaLogSourceInfo ;

extern umba::SimpleFormatter  umbaLogStreamErr;
extern umba::SimpleFormatter  umbaLogStreamMsg;
extern umba::SimpleFormatter  umbaLogStreamNul;

// Если надо будет протоколировать в какой-то отдельный поток вывода в каком-то скопе, 
// то там надо будет как-то извращаться. Но пока это обычно для утилит командной строки, 
// и достаточно глобальных переменных


// source parsing errors
// requires std::stringr curFile, unsigned lineNo in log scope
#define UMBA_LOG_ERR_INPUT                 umba::log::startLogError( umbaLogStreamErr, umba::log::LogEntryType::err ,  /* gopts, */  std::string()/*"err"*/, curFile.c_str(), lineNo, __FILE__, __LINE__ )
#define UMBA_LOG_WARN_INPUT(warnType)      umba::log::startLogError( umbaLogStreamErr, umba::log::LogEntryType::warn,  /* gopts, */  std::string(warnType) , curFile.c_str(), lineNo, __FILE__, __LINE__ )

// options and other errors
#define UMBA_LOG_ERR                       umba::log::startLogError( umbaLogStreamErr, umba::log::LogEntryType::err ,  /* gopts,  */ std::string()/*"err"*/, (const char*)0 , 0     , __FILE__, __LINE__ )
#define UMBA_LOG_WARN(warnType)            umba::log::startLogError( umbaLogStreamErr, umba::log::LogEntryType::warn,  /* gopts,  */ std::string(warnType) , (const char*)0 , 0     , __FILE__, __LINE__ )
                                                                            /*         */                                        
#define UMBA_LOG_MSG                       umba::log::startLogError( umbaLogStreamMsg, umba::log::LogEntryType::msg ,  /* gopts,  */ std::string()/*"msg"*/, (const char*)0 , 0     , (const char*)0, 0 )




// umba::log::
namespace umba {
namespace log {



enum class LogEntryType
{
    err, warn, msg
};


inline
std::unordered_map<std::string, bool>& getWarningDisabledMap()
{
    static std::unordered_map<std::string, bool> m;
    return m; 
    
    // В многопотоке могут быть проблемы.
    // С другой стороны, все настройки варнингов делаются обычно до какой-то работы
    // и в многопотоке будет только поиск
}

inline
bool isWarningDisabled(const std::string &warnType)
{
    const std::unordered_map<std::string, bool> &m = getWarningDisabledMap();
    std::unordered_map<std::string, bool>::const_iterator it = m.find(warnType);
    if (it==m.end())
        return false; // not explicitly disabled
    return it->second;
}

inline
void setWarningDisabled(const std::string &warnType, bool bDisabled=true)
{
    std::unordered_map<std::string, bool> &m = getWarningDisabledMap();
    m[warnType] = bDisabled;
}


inline
umba::SimpleFormatter& startLogError( umba::SimpleFormatter   &s
                                    , LogEntryType             logEntryType
                                    // , const GeneratorOptions &gopts
                                    , const std::string       &warnType
                                    , const char* inputFile   , unsigned inputLineNo
                                    , const char* srcFile = 0 , unsigned srcLineNo = 0
                                    )
{
    if (logEntryType==LogEntryType::warn && isWarningDisabled(warnType))
        return umbaLogStreamNul;
     

    using namespace umba::omanip;

    if (logEntryType==LogEntryType::err || logEntryType==LogEntryType::warn)
    {
        if (logEntryType==LogEntryType::err)
            s<<error;
        else
            s<<warning;
    }

    if (inputFile)
    {
        if (umbaLogGccFormat)
        {
            // ..\src\main\payloads_bus_master_task.h:60:1: error:
            if (inputFile) //-V547
                s<<inputFile<<":"<<inputLineNo<<": ";
        }
        else
        {
            // e:\_work\utils\rdlc\src\rdlc.cpp(668): error C2065:
            if (inputFile) //-V547
                s<<inputFile<<"("<<inputLineNo<<"): ";
        }

        if (logEntryType==LogEntryType::err)
        {
            s<< "error:";
        }
        else if (logEntryType==LogEntryType::warn)
        {
            s<< "warning";
            if (!warnType.empty())
            {
                // also need to check warn is enabled or not
                s<< "("<<warnType<<")";
            }
            // if (logWarnType)
            //     s<< "("<<warnType<<"):";
            s<< ":";
        }
    }
    else
    {
        if (logEntryType==LogEntryType::err)
            s<< "error:";
        else if (logEntryType==LogEntryType::warn)
        {
            s<< "warning";
            if (!warnType.empty())
            {
                // also need to check warn is enabled or not
                s<< "("<<warnType<<")";
            }
            // if (logWarnType)
            //     s<< "("<<warnType<<"):";
            s<< ":";
        }
    }

    if (srcFile && umbaLogSourceInfo)
    {
        s<<" ";
        s<<coloring( UMBA_TERM_COLORS_MAKE_COMPOSITE( umba::term::colors::blue, umba::term::colors::color_default, true  ,  false,  false ) )
         <<"["<<srcFile<<":"<<srcLineNo<<"]";
    }
    
    if (logEntryType==LogEntryType::err || logEntryType==LogEntryType::warn)
        s<<normal<<" ";

    return s;
}

} // namespace log
} // namespace umba




