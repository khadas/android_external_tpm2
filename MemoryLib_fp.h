/*
 * Copyright 2015 The Chromium OS Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef __MEMORYLIB_FP_H
#define __MEMORYLIB_FP_H

LIB_EXPORT BOOL Memory2BEqual(
        const TPM2B         *aIn,                 // IN: compare value
        const TPM2B         *bIn                  // IN: compare value
);
LIB_EXPORT void MemorySet(
    void                 *destination,           // OUT: memory destination
    char                  value,                 // IN: fill value
    UINT32                size                   // IN: number of octets to fill
                          );

#endif
