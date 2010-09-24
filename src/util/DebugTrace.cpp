#include "StdIncl.h"
#include "DebugTrace.h"
#include <cstdarg>

#include "PlatformAPI.h"

static char g_logfile[64];

void d_trace(const char *fmt, ...)
{
	static char buf[256];

	va_list ap;
	va_start(ap,fmt);
	VSNPRINTF (buf,256,fmt,ap);
	va_end(ap);

	d_puts (buf);
}

// d_puts - writes a string of any length to the debug output
// 	(as opposed to d_trace, which is limited by the buffer)
void d_puts (const char *buf)
{
	pf_DebugLog(buf);

#ifndef TARGET_OS_IPHONE
	if(g_logfile[0])
	{
		FILE *p = fopen (g_logfile, "a");
		if(p) {
			fputs(buf, p);
			fclose (p);
		}
	}
#endif
}

