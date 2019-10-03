#ifndef _STDINT_H
#define _STDINT_H

#ifndef __int8_t_defined
# define __int8_t_defined
typedef signed char             int8_t;
typedef short int               int16_t;
typedef int                     int32_t;
# if __WORDSIZE == 64
typedef long int                int64_t;
# else
__extension__
typedef long long int           int64_t;
# endif
#endif

/* Unsigned.  */
typedef unsigned char           uint8_t;
typedef unsigned short int      uint16_t;
#ifndef __uint32_t_defined
typedef unsigned int            uint32_t;
# define __uint32_t_defined
#endif
#if __WORDSIZE == 64
typedef unsigned long int       uint64_t;
#else
__extension__
typedef unsigned long long int  uint64_t;
#endif

typedef unsigned long ptrdiff_t;

/* Largest integral types.  */
#if __WORDSIZE == 64
typedef long int                intmax_t;
typedef unsigned long int       uintmax_t;
#else
__extension__
typedef long long int           intmax_t;
__extension__
typedef unsigned long long int  uintmax_t;
#endif

/* Types for `void *' pointers.  */
#if __WORDSIZE == 64
# ifndef __intptr_t_defined
typedef long int                intptr_t;
#  define __intptr_t_defined
# endif
typedef unsigned long int       uintptr_t;
#else
# ifndef __intptr_t_defined
typedef int                     intptr_t;
#  define __intptr_t_defined
# endif
typedef unsigned int            uintptr_t;
#endif

# if __WORDSIZE == 64
#  define __INT64_C(c)  c ## L
#  define __UINT64_C(c) c ## UL
# else
#  define __INT64_C(c)  c ## LL
#  define __UINT64_C(c) c ## ULL
# endif

/* Limits of integral types.  */

/* Minimum of signed integral types.  */
# define INT8_MIN               (-128)
# define INT16_MIN              (-32767-1)
# define INT32_MIN              (-2147483647-1)
# define INT64_MIN              (-__INT64_C(9223372036854775807)-1)
/* Maximum of signed integral types.  */
# define INT8_MAX               (127)
# define INT16_MAX              (32767)
# define INT32_MAX              (2147483647)
# define INT64_MAX              (__INT64_C(9223372036854775807))

/* Maximum of unsigned integral types.  */
# define UINT8_MAX              (255)
# define UINT16_MAX             (65535)
# define UINT32_MAX             (4294967295U)
# define UINT64_MAX             (__UINT64_C(18446744073709551615))


/* Minimum of signed integral types having a minimum size.  */
# define INT_LEAST8_MIN         (-128)
# define INT_LEAST16_MIN        (-32767-1)
# define INT_LEAST32_MIN        (-2147483647-1)
# define INT_LEAST64_MIN        (-__INT64_C(9223372036854775807)-1)
/* Maximum of signed integral types having a minimum size.  */
# define INT_LEAST8_MAX         (127)
# define INT_LEAST16_MAX        (32767)
# define INT_LEAST32_MAX        (2147483647)
# define INT_LEAST64_MAX        (__INT64_C(9223372036854775807))

/* Maximum of unsigned integral types having a minimum size.  */
# define UINT_LEAST8_MAX        (255)
# define UINT_LEAST16_MAX       (65535)
# define UINT_LEAST32_MAX       (4294967295U)
# define UINT_LEAST64_MAX       (__UINT64_C(18446744073709551615))
# if __WORDSIZE == 64
#  define __INT64_C(c)  c ## L
#  define __UINT64_C(c) c ## UL
# else
#  define __INT64_C(c)  c ## LL
#  define __UINT64_C(c) c ## ULL
# endif

/* Limits of integral types.  */

/* Minimum of signed integral types.  */
# define INT8_MIN               (-128)
# define INT16_MIN              (-32767-1)
# define INT32_MIN              (-2147483647-1)
# define INT64_MIN              (-__INT64_C(9223372036854775807)-1)
/* Maximum of signed integral types.  */
# define INT8_MAX               (127)
# define INT16_MAX              (32767)
# define INT32_MAX              (2147483647)
# define INT64_MAX              (__INT64_C(9223372036854775807))

/* Maximum of unsigned integral types.  */
# define UINT8_MAX              (255)
# define UINT16_MAX             (65535)
# define UINT32_MAX             (4294967295U)
# define UINT64_MAX             (__UINT64_C(18446744073709551615))


/* Minimum of signed integral types having a minimum size.  */
# define INT_LEAST8_MIN         (-128)
# define INT_LEAST16_MIN        (-32767-1)
# define INT_LEAST32_MIN        (-2147483647-1)
# define INT_LEAST64_MIN        (-__INT64_C(9223372036854775807)-1)
/* Maximum of signed integral types having a minimum size.  */
# define INT_LEAST8_MAX         (127)
# define INT_LEAST16_MAX        (32767)
# define INT_LEAST32_MAX        (2147483647)
# define INT_LEAST64_MAX        (__INT64_C(9223372036854775807))

/* Maximum of unsigned integral types having a minimum size.  */
# define UINT_LEAST8_MAX        (255)
# define UINT_LEAST16_MAX       (65535)
# define UINT_LEAST32_MAX       (4294967295U)
# define UINT_LEAST64_MAX       (__UINT64_C(18446744073709551615))





#endif
