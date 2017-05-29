#pragma once

#if defined(_WIN64) || defined(_WIN32)
#define CPF_WINDOWS_BUILD 1
#elif defined(__APPLE__)
#define CPF_MAC_BUILD 1
#elif defined(__linux__) || defined(__unix__)
#define CPF_LINUX_BUILD 1
#endif // #if defined(_WIN64) || defined(_WIN32)

#if !defined(CPF_DBG_CONFIG) /*... in compiler flags*/
#ifndef NDEBUG
#define CPF_DBG_CONFIG 1
#else
#define CPF_DBG_CONFIG 0
#endif /* NDEBUG */
#endif /* !defined(CPF_DBG_CONFIG) */

#ifdef CPF_WINDOWS_BUILD
/*
 * CPF_BUILD_AS_SHARED is defined for shared library.
 * CPF_FUNC_EXPORT  is defined for building the DLL library.
 */
#ifndef CPF_BUILD_AS_SHARED
#define CPF_API extern
#else
#ifdef CPF_FUNC_EXPORT
#define CPF_API extern __declspec(dllexport)
#else
#define CPF_API extern __declspec(dllimport)
#endif
#endif // #ifdef CPF_BUILD_AS_SHARED

#elif defined(CPF_LINUX_BUILD) /*| defined(CPF_MAC_BUILD)*/

#ifndef CPF_BUILD_AS_SHARED
#define CPF_API extern
#else
#ifdef CPF_FUNC_EXPORT
#if defined(__GNUC__) && __GNUC__ >= 4
#define CPF_API extern __attribute__((visibility("default")))
#else
#pragma warning Unknown dynamic link import / export semantics.
#endif
#else
#if defined(__GNUC__) && __GNUC__ >= 4
#define CPF_API
#else
#pragma warning Unknown dynamic link import / export semantics.
#endif // #if defined (__GNUC__) && __GNUC__>=4
#endif // #ifdef CPF_FUNC_EXPORT
#endif // #ifdef CPF_BUILD_AS_SHARED

#endif /*#ifdef CPF_WINDOWS_BUILD*/

#include <algorithm>

#define CPF_ESC_CHAR ('`')

// from narrow to wide e.g. const char* -> const wchar_t*
// string literals only.
#define CPF_WIDEN_2__(x) L##x
#define CPF_WIDEN_1__(x) CPF_WIDEN_2__(x)
#define CPF_WIDEN_STRING_LITERAL(s) CPF_WIDEN_1__(s)

/*
        error codes.
*/

// no error
#define CPF_SUCCESS (0)

// invalid token(s) encountered in format string
#define CPF_FORMAT_STRING_TOKEN_ERROR (-1)

// invalid format specifier(s) in format string
#define CPF_FORMAT_SPECIFIER_ERROR (-2)

// mismatch between a format specifier and its corresponding argument
#define CPF_ARG_ERR (-3)

// system config error
#define CPF_SYSTEM_ERR (-4)

// ascii to unicode character string conversion error.
#define CPF_NWCONV_ERR (-5)

