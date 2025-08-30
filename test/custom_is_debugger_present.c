#include <stdio.h>
#include <stdlib.h>

static int want_debugbreak_to_fire = 0;

#define USER_DEFINED_FUNC_IS_DEBUGGER_PRESENT()    (want_debugbreak_to_fire != 0)

// fix for building this sample code in monolithic mode: this next line is NOT required for regular vanilla builds of the code!
#define DEBUGBREAK_OFFLOAD_IMPLEMENTATION      0

#include "debugbreak.h"
#include "monolithic_examples.h"

#if defined(BUILD_MONOLITHIC)
#define main      dbgbrk_custom_is_debugger_present_example_main
#endif

int main(int argc, const char** argv)
{
	if (argc > 1)
	{
		want_debugbreak_to_fire = atoi(argv[1]);
	}
	printf("debugbreak is %s\n", want_debugbreak_to_fire ? "ENABLED" : "disabled");
	fflush(stdout);

	debug_break();
	printf("hello world\n");
	return 0;
}
