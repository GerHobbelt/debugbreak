#include <stdio.h>
#include "monolithic_examples.h"


#if defined(BUILD_MONOLITHIC)
#define main      dbgbrk_trap_example_main
#endif

int main(void)
{
#if !defined(_MSC_VER)
	__builtin_trap();
#else
	{
		volatile int* p = NULL;
		(void)(*p);
	}
#endif
	printf("hello world\n");
	return 0;
}
