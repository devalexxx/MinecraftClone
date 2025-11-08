// Copyright (c) 2025 devalexxx
// Distributed under the MIT License.
// https://opensource.org/licenses/MIT

#ifndef MCC_COMMON_UTILS_LOGGING_H
#define MCC_COMMON_UTILS_LOGGING_H

#include <fmt/color.h>
#include <fmt/core.h>

#include <chrono>

#define MCC_NO_LOG 0x00
#define MCC_ERROR_LEVEL 0x01
#define MCC_WARN_LEVEL 0x02
#define MCC_INFO_LEVEL 0x03
#define MCC_DEBUG_LEVEL 0x04

#define MCC_TAG_ERROR fmt::format(fg(fmt::color::red), "ERROR")
#define MCC_TAG_WARN fmt::format(fg(fmt::color::yellow), "WARN ")
#define MCC_TAG_INFO fmt::format(fg(fmt::color::cyan), "INFO ")
#define MCC_TAG_DEBUG "DEBUG"

#ifdef MCC_DEBUG
#    define MCC_LOG_LEVEL MCC_DEBUG_LEVEL
#elifndef MCC_LOG_LEVEL
#    define MCC_LOG_LEVEL MCC_ERROR_LEVEL
#endif

#define MCC_FMT_PRINT(fmt_, ...) fmt::print(fmt_ __VA_OPT__(, ) __VA_ARGS__)

#ifdef MCC_LOG_FULL
#    define MCC_LOG_FMT "{} | {} | {:<25.25} | {:<27.27}:{:<3} | "
#    define MCC_LOG_FILE strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__
#    define MCC_LOG_TIME Mcc::_::Timenow()
#    define MCC_LOG_NL "\n"
#    define MCC_LOG_ARGS(LOG_TAG) MCC_LOG_TIME, LOG_TAG, MCC_LOG_FILE, __FUNCTION__, __LINE__
#else
#    define MCC_LOG_FMT "{} | {} | "
#    define MCC_LOG_TIME Mcc::_::Timenow()
#    define MCC_LOG_NL "\n"
#    define MCC_LOG_ARGS(LOG_TAG) MCC_LOG_TIME, LOG_TAG
#endif

#if MCC_LOG_LEVEL >= MCC_DEBUG_LEVEL
#    define MCC_LOG_DEBUG(fmt, ...)                                                                       \
        MCC_FMT_PRINT(MCC_LOG_FMT fmt MCC_LOG_NL, MCC_LOG_ARGS(MCC_TAG_DEBUG) __VA_OPT__(, ) __VA_ARGS__)
#else
#    define MCC_LOG_DEBUG(fmt, ...)
#endif

#if MCC_LOG_LEVEL >= MCC_INFO_LEVEL
#    define MCC_LOG_INFO(fmt, ...)                                                                       \
        MCC_FMT_PRINT(MCC_LOG_FMT fmt MCC_LOG_NL, MCC_LOG_ARGS(MCC_TAG_INFO) __VA_OPT__(, ) __VA_ARGS__)
#else
#    define MCC_LOG_INFO(fmt, ...)
#endif

#if MCC_LOG_LEVEL >= MCC_WARN_LEVEL
#    define MCC_LOG_WARN(fmt, ...)                                                                       \
        MCC_FMT_PRINT(MCC_LOG_FMT fmt MCC_LOG_NL, MCC_LOG_ARGS(MCC_TAG_WARN) __VA_OPT__(, ) __VA_ARGS__)
#else
#    define MCC_LOG_WARN(fmt, ...)
#endif

#if MCC_LOG_LEVEL >= MCC_ERROR_LEVEL
#    define MCC_LOG_ERROR(fmt, ...)                                                                       \
        MCC_FMT_PRINT(MCC_LOG_FMT fmt MCC_LOG_NL, MCC_LOG_ARGS(MCC_TAG_ERROR) __VA_OPT__(, ) __VA_ARGS__)
#else
#    define MCC_LOG_ERROR(fmt, ...)
#endif

namespace Mcc::_
{
    inline static char* Timenow()
    {
        static char buffer[64];

        time_t rawtime;
        tm*    timeinfo;

        time(&rawtime);
        timeinfo = localtime(&rawtime);

        strftime(buffer, 64, "%Y-%m-%d %H:%M:%S", timeinfo);

        return buffer;
    }
}

#endif
