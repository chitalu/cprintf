#ifndef _CPF_TYPE_NORM_H
#define _CPF_TYPE_NORM_H

#include <string>
#include <type_traits>

template<class T>
typename std::enable_if<std::is_integral<T>::value, T>::type
normalize_arg(const T arg)
{
	return arg;
}

template<class T>
typename std::enable_if<std::is_floating_point<T>::value, T>::type
normalize_arg(const T arg)
{
	return arg;
}

template<class T>
typename std::enable_if<std::is_pointer<T>::value, T>::type
normalize_arg(const T arg)
{
	return arg;
}

extern const char* normalize_arg(const std::string& arg);

#endif