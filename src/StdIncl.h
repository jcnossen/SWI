#pragma once

#define _CRT_SECURE_NO_WARNINGS

#ifdef _MSC_VER
#pragma warning(disable: 4018) // signed/unsigned mismatch
#pragma warning(disable: 4244) // conversion from 'int' to 'float', possible loss of data
#endif

#include <string>
#include <vector>
#include <algorithm>
#include <cstdio>
#include <stdexcept>
#include <cassert>
#include <map>

#define PI 3.141593f

typedef unsigned int uint;
typedef unsigned short ushort;
typedef unsigned long ulong;
typedef unsigned char uchar;

std::string SPrintf(const char *fmt, ...);

std::string ReadZStr (FILE*f);
void WriteZStr (FILE *f, const std::string& s);

#define CONTAINS(Collection, Value) \
	(std::find((Collection).begin(), (Collection).end(), Value) != (Collection).end())

class ContentException : public std::runtime_error {
public:
	ContentException(const std::string& msg) : std::runtime_error(msg.c_str())
	{ }
	virtual ~ContentException() throw() {}
};


/*
 * Portable definition for SNPRINTF, VSNPRINTF, STRCASECMP and STRNCASECMP
 */
#ifdef _MSC_VER
	#if _MSC_VER > 1310
		#define SNPRINTF _snprintf_s
		#define VSNPRINTF _vsnprintf_s
	#else
		#define SNPRINTF _snprintf
		#define VSNPRINTF _vsnprintf
	#endif
	#define STRCASECMP _stricmp
	#define STRNCASECMP _strnicmp
	#define ALLOCA(size) _alloca(size) // allocates memory on stack
#else
	#define STRCASECMP strcasecmp
	#define STRNCASECMP strncasecmp
	#define SNPRINTF snprintf
	#define VSNPRINTF vsnprintf
	#define ALLOCA(size) alloca(size)
#endif
#define ALLOCA_ARRAY(T, N) (new(ALLOCA(sizeof(T) * N)) T[N])



/*
 * HASH_SET defines a hash map template in a portable way.
 * Use #include HASH_MAP_H to include the header
 */
#ifdef _MSC_VER
	#define HASH_SET stdext::hash_set
	#define HASH_SET_H <hash_set>

	#define HASH_MAP stdext::hash_map
	#define HASH_MAP_H <hash_map>
#elif defined(__GNUC__)
	#define HASH_SET __gnu_cxx::hash_set
	#define HASH_SET_H <ext/hash_set>

	#define HASH_MAP __gnu_cxx::hash_map
	#define HASH_MAP_H <ext/hash_map>

	// This is needed as gnu doesn't offer specialization for other pointer types other than char*
	// (look in ext/hash_fun.h for the types supported out of the box)

/*	#include <ext/hash_fun.h>

	namespace __gnu_cxx {
		template<> struct hash<void*> {
			size_t operator() (const void* __s) const { return (size_t)(__s); }
		};

// 		template<> struct hash<String> {
// 			size_t operator() (const String& __s) const { return hash<const char*>()(__s.c_str()); }
// 		};
		template<> struct hash<std::string> {
			size_t operator() (const std::string& __s) const { return hash<const char*>()(__s.c_str()); }
		};
	}*/
#else
	#error Unsupported compiler
#endif

#include "DebugTrace.h"
#include "Vector2.h"
#include "Matrix22.h"
#include "Util.h"
