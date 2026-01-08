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

#ifndef __KASAN_COMMON_H__
#define __KASAN_COMMON_H__

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifdef KASAN_ENABLED

void *__kasan_memcpy(void *dst, const void *src, unsigned int size);
void *__kasan_memset(void *buf, int c, unsigned int size);

#define memcpy(dst, src, size) __kasan_memcpy(dst, src, size)
#define memset(buf, c, size) __kasan_memset(buf, c, size)

#else  // KASAN_ENABLED

void *memcpy(void *dst, const void *src, unsigned long size);
void *memset(void *buf, int c, unsigned long size);

#endif  // KASAN_ENABLED

#endif  // __KASAN_COMMON_H__