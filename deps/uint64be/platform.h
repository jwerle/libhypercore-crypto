#ifndef UINT64BE_PLATFORM_H
#define UINT64BE_PLATFORM_H

#if defined(_WIN32)
#  define UINT64BE_EXPORT __declspec(dllimport)
#elif defined(__GNUC__) && (__GNUC__ * 100 + __GNUC_MINOR) >= 303
#  define UINT64BE_EXPORT __attribute__((visibility("default")))
#  define UINT64BE_INLINE inline
#else
#  define UINT64BE_EXPORT
#  define UINT64BE_INLINE
#endif

#ifndef UINT64BE_ALIGNMENT
#  define UINT64BE_ALIGNMENT sizeof(unsigned long) // platform word
#endif

#ifndef UINT64BE_MAX_ENUM
#  define UINT64BE_MAX_ENUM 0x7FFFFFFF
#endif

#endif
