
#ifndef MORPH_DEBUG_HPP
#define MORPH_DEBUG_HPP

#ifdef MORPH_WINDOWS
    #define MORPH_DEBUG_BREAK __debugbreak()
#elif MORPH_LINUX
    #include <signal.h>
    #define MORPH_DEBUG_BREAK raise(SIGTRAP)
#else
    #define MORPH_DEBUG_BREAK 0
#endif

#endif // MORPH_DEBUG_HPP