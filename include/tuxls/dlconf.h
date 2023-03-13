#pragma once



#pragma once

#ifdef _WIN32
#   pragma warning(disable : 4996)
//#   pragma warning(disable : 4005)
//#   pragma warning(disable : 4267)
#   pragma warning(disable : 4251)
//class __declspec(dllimport) std::thread;
#   define _CRT_SECURE_NO_WARNINGS
#   define _SCL_SECURE_NO_WARNINGS

#   ifdef tuxls_EXPORT
#       define TUXLIB __declspec(dllexport)
#   else
#       define TUXLIB __declspec(dllimport)
#   endif

#   include <Windows.h> // Must be included before anything else
#   include <shlwapi.h>
#   include <thread>
#   include <cstdint>
#   define __PRETTY_FUNCTION__ __FUNCSIG__
#else
#       define TUXLIB
#endif

//---------------------------------------------


// --  Basic pseudo reflexion macros : --
#ifndef CLASSNAME
#define CLASSNAME \
    public:\
        virtual const char*  class_name() const noexcept; \
    private:\

#define  CLASSNAME_IMPL(CLASS) \
         const char*   CLASS::class_name() const noexcept{ return #CLASS; }
#endif
//----------------------------------------------------------------------------





