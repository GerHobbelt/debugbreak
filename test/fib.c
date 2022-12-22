#include <stdio.h>

#include "debugbreak.h"
#include "monolithic_examples.h"

static int fib(int n)
{
	int r;
	if (n == 0 || n == 1)
		return 1;
	r = fib(n-1) + fib(n-2);
	if (r == 89) {
		debug_break();
	}
	return r;
}


#if defined(BUILD_MONOLITHIC)
#define main      dbgbrk_fib_example_main
#endif

int main(void)
{
	printf("%d\n", fib(15));
	return 0;
}
