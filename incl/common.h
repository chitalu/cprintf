#include <utility>
#include <vector>
#include <string>
#include <cstdio>
#include <assert.h>
#include <map>

#ifdef _WIN32
#include <Windows.h>
typedef WORD colour_t;
#else
typedef std::string colour_t;
#endif

typedef std::pair<std::string, std::string> str_pair;
/*meta format string type*/
typedef std::map<std::size_t, str_pair> meta_format_t;
typedef FILE* stream_t;