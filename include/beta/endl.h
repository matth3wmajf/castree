#pragma once

#if defined(__WIN32) || defined(__WIN64)
#define ENDL "\r\n"
#elif defined(__unix__)
#define ENDL "\n"
#else
#define ENDL "\n"
#endif