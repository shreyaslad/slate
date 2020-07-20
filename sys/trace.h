#ifndef __SYS__TRACE_H__
#define __SYS__TRACE_H__

#include <stddef.h>
#include <io.h>
#include <sys/symlist.h>

void stacktrace(size_t* base);

#endif
