/*
 * Copyright 2024 Google LLC
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

#include "kasan.h"

#include "heap.h"
#include "printf.h"
#include "rt_utils.h"
#include "sanitized_lib.h"

#include <stdint.h>

#ifdef QEMU_EXIT
#ifdef QEMU_EXIT_AARCH64
__attribute__((noreturn)) void semihost_exit(int status)
{
  struct
  {
    uint64_t reason;
    uint64_t status;
  } block = {0x20026, (uint64_t)status};

  register uint64_t x0 asm("x0") = 0x20;
  register void *x1 asm("x1") = &block;

  asm volatile(
      "hlt #0xF000"
      :
      : "r"(x0), "r"(x1)
      : "memory");

  for (;;)
    ;
}
#endif

#ifdef QEMU_EXIT_ARM
__attribute__((noreturn)) void semihost_exit(int status)
{
  // Parameter block: {Reason, Subcode/Status}
    uint32_t params[2] = {0x20026, (uint32_t)status};

    register uint32_t reg0 __asm__("r0") = 0x20; // SYS_EXIT_EXTENDED
    register uint32_t* reg1 __asm__("r1") = params;

    __asm__ volatile (
        "svc 0x123456"
        :
        : "r"(reg0), "r"(reg1)
        : "memory"
    );
}
#endif
#endif

int main(void) {
  printf("Starting bare-metal KASan test driver.\n");

  // Needed to invoke KASan globals instrumentation.
  call_global_ctors();

  initialize_heap();

  initialize_kasan();

  test_heap_overflow();
  test_stack_overflow();
  test_globals_overflow();
  test_memset_overflow();
  test_memcpy_overflow();

#ifdef QEMU_EXIT
  semihost_exit(0);
#endif

  printf("Press ctrl + a then x to exit.\n");

  return 0;
}
