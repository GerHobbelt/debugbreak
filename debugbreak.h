/* Copyright (c) 2011-2021, Scott Tsai
 * 
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef DEBUG_BREAK_H
#define DEBUG_BREAK_H

#if !defined(__has_builtin)
#define __has_builtin(f)      0
#define LIBDEBUGBREAK_EMULATES___HAS_BUILTIN   1
#endif

#if !defined(__has_include)
#define __has_include(f)      0
#define LIBDEBUGBREAK_EMULATES___HAS_INCLUDE   1
#endif

#if !defined(__has_feature)
#define __has_feature(f)      0
#define LIBDEBUGBREAK_EMULATES___HAS_FEATURE   1
#endif


// from libassert.
// 
// TODO:
// roll this into here + adjust libassert to `#include <debugbreak.h>` and use `debug_break()` afterwards: single implementation throughout the monolith...

#if 0
#if LIBASSERT_IS_CLANG || LIBASSERT_IS_GCC
#define LIBASSERT_WARNING_PRAGMA_PUSH _Pragma("GCC diagnostic push")
#define LIBASSERT_WARNING_PRAGMA_POP _Pragma("GCC diagnostic pop")
#elif LIBASSERT_IS_MSVC
#define LIBASSERT_WARNING_PRAGMA_PUSH _Pragma("warning(push)")
#define LIBASSERT_WARNING_PRAGMA_POP _Pragma("warning(pop)")
#endif

#if LIBASSERT_IS_CLANG || LIBASSERT_IS_ICX
 // clang and icx support this as far back as this library could care
#define LIBASSERT_BREAKPOINT() __builtin_debugtrap()
#elif LIBASSERT_IS_MSVC || LIBASSERT_IS_ICC
 // msvc and icc support this as far back as this library could care
#define LIBASSERT_BREAKPOINT() __debugbreak()
#elif LIBASSERT_IS_GCC
#if LIBASSERT_GCC_VERSION >= 1200
#define LIBASSERT_IGNORE_CPP20_EXTENSION_WARNING _Pragma("GCC diagnostic ignored \"-Wc++20-extensions\"")
#else
#define LIBASSERT_IGNORE_CPP20_EXTENSION_WARNING
#endif
#define LIBASSERT_ASM_BREAKPOINT(instruction) \
  do { \
   LIBASSERT_WARNING_PRAGMA_PUSH \
   LIBASSERT_IGNORE_CPP20_EXTENSION_WARNING \
   __asm__ __volatile__(instruction) \
   ; \
   LIBASSERT_WARNING_PRAGMA_POP \
  } while(0)
 // precedence for these come from llvm's __builtin_debugtrap() implementation
 // arm: https://github.com/llvm/llvm-project/blob/e9954ec087d640809082f46d1c7e5ac1767b798d/llvm/lib/Target/ARM/ARMInstrInfo.td#L2393-L2394
 //  def : Pat<(debugtrap), (BKPT 0)>, Requires<[IsARM, HasV5T]>;
 //  def : Pat<(debugtrap), (UDF 254)>, Requires<[IsARM, NoV5T]>;
 // thumb: https://github.com/llvm/llvm-project/blob/e9954ec087d640809082f46d1c7e5ac1767b798d/llvm/lib/Target/ARM/ARMInstrThumb.td#L1444-L1445
 //  def : Pat<(debugtrap), (tBKPT 0)>, Requires<[IsThumb, HasV5T]>;
 //  def : Pat<(debugtrap), (tUDF 254)>, Requires<[IsThumb, NoV5T]>;
 // aarch64: https://github.com/llvm/llvm-project/blob/e9954ec087d640809082f46d1c7e5ac1767b798d/llvm/lib/Target/AArch64/AArch64FastISel.cpp#L3615-L3618
 //  case Intrinsic::debugtrap:
 //   BuildMI(*FuncInfo.MBB, FuncInfo.InsertPt, MIMD, TII.get(AArch64::BRK))
 //       .addImm(0xF000);
 //   return true;
 // x86: https://github.com/llvm/llvm-project/blob/e9954ec087d640809082f46d1c7e5ac1767b798d/llvm/lib/Target/X86/X86InstrSystem.td#L81-L84
 //  def : Pat<(debugtrap),
 //            (INT3)>, Requires<[NotPS]>;
#if defined(__i386__) || defined(__x86_64__)
#define LIBASSERT_BREAKPOINT() LIBASSERT_ASM_BREAKPOINT("int3")
#elif defined(__arm__) || defined(__thumb__)
#if __ARM_ARCH >= 5
#define LIBASSERT_BREAKPOINT() LIBASSERT_ASM_BREAKPOINT("bkpt #0")
#else
#define LIBASSERT_BREAKPOINT() LIBASSERT_ASM_BREAKPOINT("udf #0xfe")
#endif
#elif defined(__aarch64__) || defined(_M_ARM64)
#define LIBASSERT_BREAKPOINT() LIBASSERT_ASM_BREAKPOINT("brk #0xf000")
#else
 // some architecture we aren't prepared for
#define LIBASSERT_BREAKPOINT()
#endif
#else
 // some compiler we aren't prepared for
#define LIBASSERT_BREAKPOINT()
#endif
#endif





#ifdef __cplusplus
#define DEBUGBREAK_EXTERN_C     extern "C"
#else
#define DEBUGBREAK_EXTERN_C     
#endif

#define DEBUG_BREAK_USE_TRAP_INSTRUCTION            1
#define DEBUG_BREAK_USE_BUILTIN_TRAP                2
#define DEBUG_BREAK_USE_SIGTRAP                     3
#define DEBUG_BREAK_USE_BUILTIN_DEBUGTRAP           4
#define DEBUG_BREAK_USE_SYSCALL                     5
#define DEBUG_BREAK_USE_INTRINSIC_MSVC_DEBUGBREAK   6
#define DEBUG_BREAK_USE_STD_DEBUGBREAK              7

// ---------------------------------------------------------------------------------
//
// definition of `debugbreak_is_debugger_present()`

#if defined(_WIN32)

//#include <winsock2.h>
//#include <windows.h>

#if !defined(_APISETDEBUG_) || !defined(WINAPI_FAMILY_PARTITION) /* || !WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_APP | WINAPI_PARTITION_SYSTEM | WINAPI_PARTITION_GAMES) */
// mandatory Win32 APIs not defined yet, so we do it here:

// #if (defined(_WIN32_WINNT) && (_WIN32_WINNT >= 0x0400)) || (defined(_WIN32_WINDOWS) && (_WIN32_WINDOWS > 0x0400))

DEBUGBREAK_EXTERN_C
extern __declspec(dllimport) void __stdcall DebugBreak(void);
DEBUGBREAK_EXTERN_C
extern __declspec(dllimport)  int __stdcall IsDebuggerPresent(void);

#endif

DEBUGBREAK_EXTERN_C
static inline int debugbreak_is_debugger_present(void)
{
	return IsDebuggerPresent();
}

#elif __has_include(<debugging>) && __has_feature(__cpp_lib_debugging)

#include <debugging>

DEBUGBREAK_EXTERN_C
static inline int debugbreak_is_debugger_present(void)
{
	return std::is_debugger_present();
}

#elif (defined(__APPLE__) || defined(__apple__)) && defined(__apple_build_version__)

// pick up IsDebuggerPresent()-similes for other platforms from Catch2, et al...

#  include <cassert>
#  include <sys/types.h>
#  include <unistd.h>
#  include <cstddef>
#  include <ostream>

// These headers will only compile with AppleClang (XCode)
// For other compilers (Clang, GCC, ... ) we need to exclude them
#  include <sys/sysctl.h>

// The following function is taken directly from the following technical note:
// https://developer.apple.com/library/archive/qa/qa1361/_index.html

// Returns true if the current process is being debugged (either
// running under the debugger or has a debugger attached post facto).
DEBUGBREAK_EXTERN_C
static inline int debugbreak_is_debugger_present(void)
{
	int                 mib[4];
	struct kinfo_proc   info;
	std::size_t         size;

	// Initialize the flags so that, if sysctl fails for some bizarre
	// reason, we get a predictable result.

	info.kp_proc.p_flag = 0;

	// Initialize mib, which tells sysctl the info we want, in this case
	// we're looking for information about a specific process ID.

	mib[0] = CTL_KERN;
	mib[1] = KERN_PROC;
	mib[2] = KERN_PROC_PID;
	mib[3] = getpid();

	// Call sysctl.

	size = sizeof(info);
	if (sysctl(mib, sizeof(mib) / sizeof(*mib), &info, &size, nullptr, 0) != 0) {
		//Catch::cerr() << "\n** Call to sysctl failed - unable to determine if debugger is active **\n\n" << std::flush;
		return false;
	}

	// We're being debugged if the P_TRACED flag is set.

	return ((info.kp_proc.p_flag & P_TRACED) != 0);
}

#elif defined(linux) || defined(__linux) || defined(__linux__)

// pick up IsDebuggerPresent()-similes for other platforms from Catch2, et al...

// The standard POSIX way of detecting a debugger is to attempt to
// ptrace() the process, but this needs to be done from a child and not
// this process itself to still allow attaching to this process later
// if wanted, so is rather heavy. Under Linux we have the PID of the
// "debugger" (which doesn't need to be gdb, of course, it could also
// be strace, for example) in /proc/$PID/status, so just get it from
// there instead.
//
// https://stackoverflow.com/questions/3596781/how-to-detect-if-the-current-process-is-being-run-by-gdb
// and the Catch2 codebase: we're going for the C implementation as that works better for our header-only approach.

#if 0   // Catch2 code
#include <fstream>
#include <string>

DEBUGBREAK_EXTERN_C
static inline int debugbreak_is_debugger_present(void)
{
	// Libstdc++ has a bug, where std::ifstream sets errno to 0
	// This way our users can properly assert over errno values
	ErrnoGuard guard;
	std::ifstream in("/proc/self/status");
	for (std::string line; std::getline(in, line); ) {
		static const int PREFIX_LEN = 11;
		if (line.compare(0, PREFIX_LEN, "TracerPid:\t") == 0) {
			// We're traced if the PID is not 0 and no other PID starts
			// with 0 digit, so it's enough to check for just a single
			// character.
			return line.length() > PREFIX_LEN && line[PREFIX_LEN] != '0';
		}
	}

	return false;
}

#else // S.O. + fixes from SO comments

#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>

DEBUGBREAK_EXTERN_C
static inline int debugbreak_is_debugger_present(void)
{
	char buf[4096];

	const int status_fd = open("/proc/self/status", O_RDONLY);
	if (status_fd == -1)
		return false;

	const ssize_t num_read = read(status_fd, buf, sizeof(buf) - 1);
	close(status_fd);

	if (num_read <= 0)
		return false;

	buf[num_read] = '\0';
	constexpr char tracerPidString[] = "TracerPid:";
	const auto tracer_pid_ptr = strstr(buf, tracerPidString);
	if (!tracer_pid_ptr)
		return false;

	for (const char* characterPtr = tracer_pid_ptr + sizeof(tracerPidString) - 1; characterPtr <= buf + num_read; ++characterPtr)
	{
		if (isspace(*characterPtr))
			continue;
		else
			return isdigit(*characterPtr) != 0 && *characterPtr != '0';
	}

	return false;
}

#endif

#else

// we don't know how to check if debugger is attached for this platform:
// --> be conservative:
// apply a heuristic: debug builds expect debugging, release builds DO NOT.
DEBUGBREAK_EXTERN_C
static inline int debugbreak_is_debugger_present(void)
{
#if defined(_DEBUG)
	return true;
#else
	return false;
#endif
}

#endif




// ---------------------------------------------------------------------------------
//
// preparation for the definition of `debug_break()`

#if __has_include(<debugging>) && __has_feature(__cpp_lib_debugging)
	#include <debugging>

	#define DEBUG_BREAK_IMPL DEBUG_BREAK_USE_STD_DEBUGBREAK
#elif __has_builtin(__builtin_debugtrap)
	#define DEBUG_BREAK_IMPL DEBUG_BREAK_USE_BUILTIN_DEBUGTRAP
#elif defined(_MSC_VER)
	#include <intrin.h>

	#pragma intrinsic(__debugbreak)

	#define DEBUG_BREAK_IMPL DEBUG_BREAK_USE_INTRINSIC_MSVC_DEBUGBREAK
#elif defined(__i386__) || defined(__x86_64__)
	#define DEBUG_BREAK_IMPL DEBUG_BREAK_USE_TRAP_INSTRUCTION

DEBUGBREAK_EXTERN_C
__inline__ static void trap_instruction(void)
{
	__asm__ volatile("int $0x03");
}
#elif defined(__linux__) && (defined(__arm__) || defined(__thumb__))
	// precedence for these come from llvm's __builtin_debugtrap() implementation
	// arm: https://github.com/llvm/llvm-project/blob/e9954ec087d640809082f46d1c7e5ac1767b798d/llvm/lib/Target/ARM/ARMInstrInfo.td#L2393-L2394
	//  def : Pat<(debugtrap), (BKPT 0)>, Requires<[IsARM, HasV5T]>;
	//  def : Pat<(debugtrap), (UDF 254)>, Requires<[IsARM, NoV5T]>;
	// thumb: https://github.com/llvm/llvm-project/blob/e9954ec087d640809082f46d1c7e5ac1767b798d/llvm/lib/Target/ARM/ARMInstrThumb.td#L1444-L1445
	//  def : Pat<(debugtrap), (tBKPT 0)>, Requires<[IsThumb, HasV5T]>;
	//  def : Pat<(debugtrap), (tUDF 254)>, Requires<[IsThumb, NoV5T]>;
	// aarch64: https://github.com/llvm/llvm-project/blob/e9954ec087d640809082f46d1c7e5ac1767b798d/llvm/lib/Target/AArch64/AArch64FastISel.cpp#L3615-L3618
	//  case Intrinsic::debugtrap:
	//   BuildMI(*FuncInfo.MBB, FuncInfo.InsertPt, MIMD, TII.get(AArch64::BRK))
	//       .addImm(0xF000);
	//   return true;
	// x86: https://github.com/llvm/llvm-project/blob/e9954ec087d640809082f46d1c7e5ac1767b798d/llvm/lib/Target/X86/X86InstrSystem.td#L81-L84
	//  def : Pat<(debugtrap),
	//            (INT3)>, Requires<[NotPS]>;
#if (__ARM_ARCH >= 5)
#define DEBUG_BREAK_IMPL DEBUG_BREAK_USE_TRAP_INSTRUCTION

DEBUGBREAK_EXTERN_C
__attribute__((always_inline))
__inline__ static void trap_instruction(void)
{
	// SEGGER fix derived from https://github.com/scottt/debugbreak/issues/20
#if defined(__GNUC__) || !defined(__BKPT)
	__asm__ volatile("bkpt #0");
#else
	__BKPT(0)
#endif
}
#else // __ARM_ARCH >= 5
#define DEBUG_BREAK_IMPL DEBUG_BREAK_USE_TRAP_INSTRUCTION

DEBUGBREAK_EXTERN_C
__attribute__((always_inline))
__inline__ static void trap_instruction(void)
{
	__asm__ volatile("udf #0xfe");
}
#endif // __ARM_ARCH >= 5
#elif defined(__thumb__)
	#define DEBUG_BREAK_IMPL DEBUG_BREAK_USE_TRAP_INSTRUCTION
/* FIXME: handle __THUMB_INTERWORK__ */
DEBUGBREAK_EXTERN_C
__attribute__((always_inline))
__inline__ static void trap_instruction(void)
{
	/* See 'arm-linux-tdep.c' in GDB source.
	 * Both instruction sequences below work. */
#if 1
	/* 'eabi_linux_thumb_le_breakpoint' */
	__asm__ volatile(".inst 0xde01");
#else
	/* 'eabi_linux_thumb2_le_breakpoint' */
	__asm__ volatile(".inst.w 0xf7f0a000");
#endif

	/* Known problem:
	 * After a breakpoint hit, can't 'stepi', 'step', or 'continue' in GDB.
	 * 'step' would keep getting stuck on the same instruction.
	 *
	 * Workaround: use the new GDB commands 'debugbreak-step' and
	 * 'debugbreak-continue' that become available
	 * after you source the script from GDB:
	 *
	 * $ gdb -x debugbreak-gdb.py <... USUAL ARGUMENTS ...>
	 *
	 * 'debugbreak-step' would jump over the breakpoint instruction with
	 * roughly equivalent of:
	 * (gdb) set $instruction_len = 2
	 * (gdb) tbreak *($pc + $instruction_len)
	 * (gdb) jump   *($pc + $instruction_len)
	 */
}
#elif defined(__arm__) && !defined(__thumb__)
	#define DEBUG_BREAK_IMPL DEBUG_BREAK_USE_TRAP_INSTRUCTION
DEBUGBREAK_EXTERN_C
__attribute__((always_inline))
__inline__ static void trap_instruction(void)
{
	/* See 'arm-linux-tdep.c' in GDB source,
	 * 'eabi_linux_arm_le_breakpoint' */
	__asm__ volatile(".inst 0xe7f001f0");
	/* Known problem:
	 * Same problem and workaround as Thumb mode */
}
#elif defined(__aarch64__) || defined(_M_ARM64)
#define DEBUG_BREAK_IMPL DEBUG_BREAK_USE_TRAP_INSTRUCTION
DEBUGBREAK_EXTERN_C
__attribute__((always_inline))
__inline__ static void trap_instruction(void)
{
	__asm__ volatile("brk #0xf000");
}
#elif defined(__aarch64__) && defined(__linux__)
	#define DEBUG_BREAK_IMPL DEBUG_BREAK_USE_SYSCALL
#define INLINE_SYS_RAISE(x) do \
	{ \
		unsigned long long raise_sig = (x); \
		__asm__ volatile ( \
			"mov x8,#178\n\t" /* gettid */ \
			"svc 0\n\t" \
			"mov x1, x0\n\t" \
			"mov x8,#172\n\t" /* getpid */ \
			"svc 0\n\t" \
			"mov x8,#131\n\t" /* tgkill */ \
			"mov x2,%0\n\t" \
			"svc 0\n\t" \
			: \
			: "r"(raise_sig) \
			: "x0", "x1", "x2", "x8", "memory", "cc" \
		); \
	} while(0)
#elif defined(__aarch64__)
	#define DEBUG_BREAK_IMPL DEBUG_BREAK_USE_TRAP_INSTRUCTION
DEBUGBREAK_EXTERN_C
__attribute__((always_inline))
__inline__ static void trap_instruction(void)
{
	/* See 'aarch64-tdep.c' in GDB source,
	 * 'aarch64_default_breakpoint' */
	__asm__ volatile(".inst 0xd4200000");
}
#elif defined(__powerpc__)
	/* PPC 32 or 64-bit, big or little endian */
	#define DEBUG_BREAK_IMPL DEBUG_BREAK_USE_TRAP_INSTRUCTION
DEBUGBREAK_EXTERN_C
__attribute__((always_inline))
__inline__ static void trap_instruction(void)
{
	/* See 'rs6000-tdep.c' in GDB source,
	 * 'rs6000_breakpoint' */
	__asm__ volatile(".4byte 0x7d821008");

	/* Known problem:
	 * After a breakpoint hit, can't 'stepi', 'step', or 'continue' in GDB.
	 * 'step' stuck on the same instruction ("twge r2,r2").
	 *
	 * The workaround is the same as ARM Thumb mode: use debugbreak-gdb.py
	 * or manually jump over the instruction. */
}
#elif defined(__riscv)
	/* RISC-V 32 or 64-bit, whether the "C" extension
	 * for compressed, 16-bit instructions are supported or not */
	#define DEBUG_BREAK_IMPL DEBUG_BREAK_USE_TRAP_INSTRUCTION
DEBUGBREAK_EXTERN_C
__attribute__((always_inline))
__inline__ static void trap_instruction(void)
{
	/* See 'riscv-tdep.c' in GDB source,
	 * 'riscv_sw_breakpoint_from_kind' */
	__asm__ volatile(".4byte 0x00100073");
}
#elif defined(__loongarch64)
	#define DEBUG_BREAK_IMPL DEBUG_BREAK_USE_TRAP_INSTRUCTION
DEBUGBREAK_EXTERN_C
__attribute__((always_inline))
__inline__ static void trap_instruction(void)
{
       __asm__ volatile("break 0x5");
}
#else
	#define DEBUG_BREAK_IMPL DEBUG_BREAK_USE_SIGTRAP
#endif


// ---------------------------------------------------------------------------------
//
// the actual definition of `debug_break()`

#ifndef DEBUG_BREAK_IMPL
#error "debugbreak.h is not supported on this target"
#elif DEBUG_BREAK_IMPL == DEBUG_BREAK_USE_SYSCALL
DEBUGBREAK_EXTERN_C
__attribute__((always_inline))
__inline__ static void debug_break(void)
{
	if (debugbreak_is_debugger_present())
	{
		/* SIGTRAP */
		INLINE_SYS_RAISE(5);
	}
}
#elif DEBUG_BREAK_IMPL == DEBUG_BREAK_USE_TRAP_INSTRUCTION
DEBUGBREAK_EXTERN_C
__attribute__((always_inline))
__inline__ static void debug_break(void)
{
	if (debugbreak_is_debugger_present())
	{
		trap_instruction();
	}
}
#elif DEBUG_BREAK_IMPL == DEBUG_BREAK_USE_BUILTIN_DEBUGTRAP
DEBUGBREAK_EXTERN_C
__attribute__((always_inline))
__inline__ static void debug_break(void)
{
	if (debugbreak_is_debugger_present())
	{
		__builtin_debugtrap();
	}
}
#elif DEBUG_BREAK_IMPL == DEBUG_BREAK_USE_BUILTIN_TRAP
DEBUGBREAK_EXTERN_C
__attribute__((always_inline))
__inline__ static void debug_break(void)
{
	if (debugbreak_is_debugger_present())
	{
		__builtin_trap();
	}
}
#elif DEBUG_BREAK_IMPL == DEBUG_BREAK_USE_SIGTRAP
#include <signal.h>

DEBUGBREAK_EXTERN_C
__attribute__((always_inline))
__inline__ static void debug_break(void)
{
	if (debugbreak_is_debugger_present())
	{
		raise(SIGTRAP);
	}
}
#elif DEBUG_BREAK_IMPL == DEBUG_BREAK_USE_INTRINSIC_MSVC_DEBUGBREAK
DEBUGBREAK_EXTERN_C
static inline void debug_break(void)
{
	if (debugbreak_is_debugger_present())
	{
		__debugbreak();
	}
}
#elif DEBUG_BREAK_IMPL == DEBUG_BREAK_USE_STD_DEBUGBREAK
DEBUGBREAK_EXTERN_C
static inline void debug_break(void)
{
	std::breakpoint_if_debugging();
}
#else
#error "invalid DEBUG_BREAK_IMPL value"
#endif

// undo the __has_feature emulation as other libraries/sources will VERY PROBABLY use `defined(__has_feature)` as a feature test, which we MUST NOT thwart!
#if defined(LIBDEBUGBREAK_EMULATES___HAS_FEATURE)
#undef __has_feature
#endif

// undo the __has_include emulation as other libraries/sources will VERY PROBABLY use `defined(__has_include)` as a feature test, which we MUST NOT thwart!
#if defined(LIBDEBUGBREAK_EMULATES___HAS_INCLUDE)
#undef __has_include
#endif

// undo the __has_builtin emulation as other libraries/sources will VERY PROBABLY use `defined(__has_builtin)` as a feature test, which we MUST NOT thwart!
#if defined(LIBDEBUGBREAK_EMULATES___HAS_BUILTIN)
#undef __has_builtin
#endif

#endif /* ifndef DEBUG_BREAK_H */
