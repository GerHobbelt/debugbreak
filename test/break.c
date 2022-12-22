#include <stdio.h>
#include "debugbreak.h"
#include "monolithic_examples.h"


#if defined(BUILD_MONOLITHIC)
#define main      dbgbrk_basic_example_main
#endif

int main(void)
{
	debug_break();
	printf("hello world\n");
	return 0;
}
