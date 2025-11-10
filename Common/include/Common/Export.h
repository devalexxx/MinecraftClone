// Copyright (c) 2025 devalexxx
// Distributed under the MIT License.
// https://opensource.org/licenses/MIT

#ifndef MCC_COMMON_EXPORT_H
#define MCC_COMMON_EXPORT_H

#if defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
#    define MCC_LIB_IMPORT __declspec(dllimport)
#    define MCC_LIB_EXPORT __declspec(dllexport)
#else
#    define MCC_LIB_IMPORT __attribute__((visibility("default")))
#    define MCC_LIB_EXPORT __attribute__((visibility("default")))
#endif

#ifdef MCC_LIB_SHARED
#    ifdef MCC_LIB_BUILD
#        define MCC_LIB_API MCC_LIB_EXPORT
#    else
#        define MCC_LIB_API MCC_LIB_IMPORT
#    endif
#else
#    define MCC_LIB_API
#endif

#endif
