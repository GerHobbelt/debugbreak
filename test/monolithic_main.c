
#define BUILD_MONOLITHIC 1
#include "monolithic_examples.h"

#define USAGE_NAME   "debugbreak_samples"

#include "monolithic_main_internal_defs.h"

MONOLITHIC_CMD_TABLE_START()

	{ "basic", {.f = dbgbrk_basic_example_main } },
	{ "fib", {.f = dbgbrk_fib_example_main } },
	{ "trap", {.f = dbgbrk_trap_example_main } },
	{ "cpp", {.f = dbgbrk_cplusplus_example_main } },

MONOLITHIC_CMD_TABLE_END();

// load the monolithic core dispatcher
#include "monolithic_main_tpl.h"
