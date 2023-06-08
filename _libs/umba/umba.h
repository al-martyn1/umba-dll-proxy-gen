#pragma once

//----------------------------------------------------------------------------

/*! 
    \file
    \brief Подключение базовых файлов UMBA
 */

//----------------------------------------------------------------------------

/*! 
    \addtogroup UMBA_CORE             Библиотека велосипедов UMBA. Базовое ядро
    \addtogroup UMBA_INTERNALS        Разные кишочки UMBA Core
    \addtogroup UMBA_LINEINFO         Немодифицирующий view-like разбор текстовых файлов
    \addtogroup UMBA_STRING_PLUS      Разное со строками

    Кое-что про UMBA (from \b \c umba.h) - \\defgroup UMBA_CORE

*/

//----------------------------------------------------------------------------

//! Простанство имён библиотеки UMBA
namespace umba{}

#include "zz_detect_environment.h"
//
#include "preprocessor.h"
//
#include "assert.h"
//
#include "autolock.h"
//
#include "critical_section.h"
//
#include "umba_cpp.h"