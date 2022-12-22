
#pragma once

#if defined(BUILD_MONOLITHIC)

#ifdef __cplusplus
extern "C" {
#endif

	int dbgbrk_basic_example_main(void);
	int dbgbrk_fib_example_main(void);
	int dbgbrk_trap_example_main(void);
	int dbgbrk_cplusplus_example_main(void);

#ifdef __cplusplus
}
#endif

#endif
