// Copyright (c) 2025 devalexxx
// Distributed under the MIT License.
// https://opensource.org/licenses/MIT

#ifndef MCC_COMMON_UTILS_ASSERT_H
#define MCC_COMMON_UTILS_ASSERT_H

#include <fmt/color.h>
#include <fmt/core.h>

#ifdef _MSC_VER
#    define PRETTY_FUNCTION __FUNCSIG__
#else
#    define PRETTY_FUNCTION __PRETTY_FUNCTION__
#endif

#define MCC_ASSERT_IMPL(fatal, expr, format, ...)                                                                   \
    (!(expr))                                                                                                       \
        ? (fmt::print(                                                                                              \
               stderr, fg(fmt::color::red), "Assertion failed at {}:{} : {}\n", __FILE__, __LINE__, PRETTY_FUNCTION \
           ),                                                                                                       \
           fmt::print(stderr, fg(fmt::color::yellow), "\tassert  : \"{}\"\n\tmessage : ", #expr),                   \
           fmt::print(stderr, fg(fmt::color::yellow), format, ##__VA_ARGS__), fmt::print(stderr, "\n"),             \
           (fatal ? abort() : (void) 0), 0)                                                                         \
        : 1

#ifdef MCC_DEBUG
#    define MCC_ASSERT(expr, fmt, ...) MCC_ASSERT_IMPL(true, expr, fmt, ##__VA_ARGS__)
#elif MCC_RELEASE_DEBUG
#    define MCC_ASSERT(expr, fmt, ...) MCC_ASSERT_IMPL(false, expr, fmt, ##__VA_ARGS__)
#else
#    define MCC_ASSERT(expr, fmt, ...)
#endif

#endif
