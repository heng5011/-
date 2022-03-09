/*
The MIT License

Copyright (c) 2007-2010 Aidin Abedi http://code.google.com/p/shinyprofiler/

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#ifndef SHINY_PREREQS_H
#define SHINY_PREREQS_H

#include "ShinyConfig.h"
#include "ShinyVersion.h"

#ifdef __cplusplus
extern "C" {
#endif


/*---------------------------------------------------------------------------*/

#define SHINY_PLATFORM_WIN32	0x1
#define SHINY_PLATFORM_POSIX	0x2

#if defined (_WIN32)
#   define SHINY_PLATFORM	SHINY_PLATFORM_WIN32

#else /* ASSUME: POSIX-compliant OS */
#   define SHINY_PLATFORM	SHINY_PLATFORM_POSIX
#endif


/*---------------------------------------------------------------------------*/

#ifndef FALSE
#define FALSE	0x0
#endif

#ifndef TRUE
#define TRUE	0x1
#endif

#ifndef NULL
#define NULL	0
#endif


/*---------------------------------------------------------------------------*/

#define SHINY_COMPILER_MSVC      0x1
#define SHINY_COMPILER_GNUC      0x2
#define SHINY_COMPILER_CLANG     0x3
#define SHINY_COMPILER_OTHER     0x4

#if defined (_MSC_VER)
#   define SHINY_COMPILER	SHINY_COMPILER_MSVC
#   define SHINY_COMPILER_GNUC_COMPAT FALSE
#   if _MSC_VER >= 1900
#      define SHINY_HAS_THEAD_LOCAL TRUE
#   endif

#elif defined (__clang__)
#   define SHINY_COMPILER    SHINY_COMPILER_CLANG
#   define SHINY_COMPILER_GNUC_COMPAT TRUE
#   define SHINY_HAS_THEAD_LOCAL TRUE

#elif defined (__GNUC__)
#   define SHINY_COMPILER	SHINY_COMPILER_GNUC
#   define SHINY_COMPILER_GNUC_COMPAT TRUE
#   define SHINY_HAS_THEAD_LOCAL TRUE

#else
#   define SHINY_COMPILER	SHINY_COMPILER_OTHER
#   define SHINY_COMPILER_GNUC_COMPAT FALSE
#   define SHINY_HAS_THEAD_LOCAL FALSE
#endif

/*---------------------------------------------------------------------------*/

#if SHINY_HAS_THEAD_LOCAL == TRUE
// 	thread_local 是 C++ 11 新引入的一种存储类型，它会影响变量的存储周期，一般用于需要保证线程安全的函数中
//	#define X 10
#   define SHINY_TLS_C thread_local
#else
#   define SHINY_TLS_C
#   if SHINY_COMPILER == SHINY_COMPILER_MSVC
#       pragma message ("Shiny is not thread-safe with this compiler")
#   else
#       warning "Shiny is not thread-safe with this compiler"
#   endif
#endif


/*---------------------------------------------------------------------------*/

#ifdef SHINY_COMPILER_GNUC_COMPAT
#include <sys/types.h>
#include <stdint.h>
#endif


/*---------------------------------------------------------------------------*/

#if SHINY_IS_COMPILED == TRUE
	struct _ShinyNode;
	struct _ShinyZone;

	typedef struct _ShinyNode* ShinyNodeCache;
	typedef struct _ShinyNode* ShinyNodeTable;
#endif


/*---------------------------------------------------------------------------*/

#if SHINY_STATIC_LINK == TRUE
#	define SHINY_API
#else
#	define SHINY_API	SHINY_EXPORT
#endif


/*---------------------------------------------------------------------------*/

#if SHINY_COMPILER == SHINY_COMPILER_MSVC
#	define SHINY_INLINE		__inline
#	define SHINY_UNUSED
#	define SHINY_EXPORT		__declspec(dllexport)

#elif SHINY_COMPILER == SHINY_COMPILER_GNUC
#	define SHINY_INLINE		static inline
/* __attribute__((unused)) 在 C程序中，如果定义了一个静态函数，而没有去使用，编译时会有一个告警
int main(void) {
    printf("main\n");
}

static void a(void) {
    printf("a\n");
}
而使用attribute((unused))可以告诉编译器忽略此告警：

__attribute__((unused)) static void a(void)
{
    printf("a\n");
}

attribute机制
*/
#	define SHINY_UNUSED		__attribute__((unused))
#	define SHINY_EXPORT		__attribute__((dllexport))

#elif SHINY_COMPILER == SHINY_COMPILER_CLANG
#	define SHINY_INLINE		static inline
#	define SHINY_UNUSED		__attribute__((unused))
#	define SHINY_EXPORT		__attribute__((dllexport))

#elif SHINY_COMPILER == SHINY_COMPILER_OTHER
#	define SHINY_INLINE		inline
#	define SHINY_UNUSED
#	define SHINY_EXPORT		extern
#endif


/*---------------------------------------------------------------------------*/

#if SHINY_COMPILER == SHINY_COMPILER_MSVC
	typedef int			int32_t;
	typedef unsigned int		uint32_t;

	typedef __int64			int64_t;
	typedef unsigned __int64	uint64_t;

/*
#elif defined(__CYGWIN__)
	typedef u_int32_t		uint32_t;
	typedef u_int64_t		uint64_t;
*/
#endif

	typedef uint64_t		shinytick_t;	//unsigned long int


#ifdef __cplusplus
} /* end of extern "C" */
#endif

#endif /* end of include guard */
