#include <iostream>
#include "debugbreak.h"
#include "monolithic_examples.h"


#if defined(BUILD_MONOLITHIC)
#define main      dbgbrk_cplusplus_example_main
#endif

int main(void)
{
	debug_break();
	std::cout << "hello, world\n";
	return 0;
}
