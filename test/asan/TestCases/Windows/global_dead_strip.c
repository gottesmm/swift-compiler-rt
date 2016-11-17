// RUN: %clang_cl_asan /O0 %s /Fe%t.exe
// RUN: %env_asan_opts=report_globals=2 %t.exe 2>&1 | FileCheck %s --check-prefix=NOSTRIP
// RUN: %clang_cl_asan /O2 %s /Fe%t.exe -link -opt:ref
// RUN: %env_asan_opts=report_globals=2 %t.exe 2>&1 | FileCheck %s --check-prefix=STRIP

// FIXME: Remove the XFAIL once the LLVM instrumentation change lands.
// XFAIL: *

#include <stdio.h>
int dead_global = 42;
int live_global = 0;
int main() {
  puts("main");
  return live_global;
}

// Check that our global registration scheme works with MSVC's linker dead
// stripping (/OPT:REF).

// NOSTRIP: Added Global{{.*}}name=dead_global
// NOSTRIP: Added Global{{.*}}name=live_global
// NOSTRIP: main

// STRIP-NOT: Added Global{{.*}}name=dead_global
// STRIP: Added Global{{.*}}name=live_global
// STRIP: main
