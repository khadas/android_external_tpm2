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
#define MemoryCopy(destination, source, size, destSize)                          \
       MemoryMove((destination), (source), (size), (destSize))
LIB_EXPORT INT16 MemoryCopy2B(
        TPM2B               *dest,                // OUT: receiving TPM2B
        const TPM2B         *source,              // IN: source TPM2B
        UINT16               dSize                // IN: size of the receiving buffer
);
LIB_EXPORT void MemoryMove(
        void              *destination,          //   OUT: move destination
        const void        *source,               //   IN: move source
        UINT32             size,                 //   IN: number of octets to moved
        UINT32             dSize                 //   IN: size of the receive buffer
                           );
UINT16 MemoryRemoveTrailingZeros (
        TPM2B_AUTH        *auth               // IN/OUT: value to adjust
);
LIB_EXPORT void MemorySet(
    void                 *destination,           // OUT: memory destination
    char                  value,                 // IN: fill value
    UINT32                size                   // IN: number of octets to fill
                          );

#endif
