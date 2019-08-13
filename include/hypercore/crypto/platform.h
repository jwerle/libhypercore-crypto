#ifndef HYPERCORE_CRYPTO_PLATFORM_H
#define HYPERCORE_CRYPTO_PLATFORM_H

#if defined(_WIN32)
#  define HYPERCORE_CRYPTO_EXPORT __declspec(dllimport)
#elif defined(__GNUC__) && (__GNUC__ * 100 + __GNUC_MINOR) >= 303
#  define HYPERCORE_CRYPTO_EXPORT __attribute__((visibility("default")))
#  define HYPERCORE_CRYPTO_INLINE inline
#else
#  define HYPERCORE_CRYPTO_EXPORT
#  define HYPERCORE_CRYPTO_INLINE
#endif

#ifndef HYPERCORE_CRYPTO_ALIGNMENT
#  define HYPERCORE_CRYPTO_ALIGNMENT sizeof(unsigned long) // platform word
#endif

#ifndef HYPERCORE_CRYPTO_MAX_ENUM
#  define HYPERCORE_CRYPTO_MAX_ENUM 0x7FFFFFFF
#endif

#endif
