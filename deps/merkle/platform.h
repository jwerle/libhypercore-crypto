#ifndef MERKLE_PLATFORM_H
#define MERKLE_PLATFORM_H

#if defined(_WIN32)
#  define MERKLE_EXPORT __declspec(dllimport)
#elif defined(__GNUC__) && (__GNUC__ * 100 + __GNUC_MINOR) >= 303
#  define MERKLE_EXPORT __attribute__((visibility("default")))
#  define MERKLE_INLINE inline
#else
#  define MERKLE_EXPORT
#  define MERKLE_INLINE
#endif

#ifndef MERKLE_ALIGNMENT
#  define MERKLE_ALIGNMENT sizeof(unsigned long) // platform word
#endif

#ifndef MERKLE_MAX_ENUM
#  define MERKLE_MAX_ENUM 0x7FFFFFFF
#endif

#endif
