// debug.c
// debug用

#include <stdio.h>
#include <stdarg.h>

void cerr(const char *format, ...)
// stderrに出力
{
	va_list arg_ptr;
	va_start(arg_ptr, format);
	vfprintf(stderr, format, arg_ptr);
	va_end(arg_ptr);
}
