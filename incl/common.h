#include <utility>
#include <vector>
#include <string>
#include <cstdio>
#include <assert.h>

#ifdef _WIN32
#include <Windows.h>
typedef WORD colour_t;
#else
typedef std::string colour_t;
#endif

/*meta format string type*/
typedef std::vector<std::pair<std::string, std::string>> meta_format_t;
typedef FILE* stream_t;