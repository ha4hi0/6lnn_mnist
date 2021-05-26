#include <stdio.h>
#include <stdarg.h>

//void cerr(const char *format, ...)
void cerr(const char *format, va_list arg_ptr)
// stderrに出力
{
	fprintf(stderr, format, arg_ptr);
}
