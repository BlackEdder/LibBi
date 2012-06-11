/**
 * @file
 *
 * Macros for compiler specific language elements.
 *
 * @author Lawrence Murray <lawrence.murray@csiro.au>
 * $Rev$
 * $Date$
 */
#ifndef BI_MISC_COMPILE_HPP
#define BI_MISC_COMPILE_HPP

/**
 * @def BI_FORCE_INLINE
 *
 * Force inlining of particular function with gcc or Intel compiler.
 */
#if defined(__GNUC__)
#define BI_FORCE_INLINE __attribute((always_inline))
#elif defined(__ICC)
#define BI_FORCE_INLINE __forceinline
#else
#define BI_FORCE_INLINE
#endif

/**
 * @def BI_ALIGN
 *
 * Align variable in memory.
 *
 * @arg n Number of bytes to which to align.
 */
#if defined(__GNUC__)
#define BI_ALIGN(n) __attribute((aligned (n)))
#elif defined(__ICC)
#define BI_ALIGN(n) __attribute((aligned (n)))
#else
#define BI_ALIGN(n)
#endif

/**
 * @def BI_UNUSED
 *
 * Declare variable as possibly unused, to eliminate warnings.
 */
#if defined(__GNUC__)
#define BI_UNUSED __attribute((unused))
#else
#define BI_UNUSED
#endif

/**
 * @def BI_THREAD
 *
 * Declare variable as thread local storage.
 */
#define BI_THREAD __thread

#endif