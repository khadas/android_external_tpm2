/*
 * Copyright 2015 The Chromium OS Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef __TPM2_NV_FP_H
#define __TPM2_NV_FP_H

void NvFlushHierarchy(
        TPMI_RH_HIERARCHY         hierarchy          // IN: hierarchy to be flushed.
);
TPM_RC NvIsAvailable(
        void
);
void NvWriteReserved(
        NV_RESERVE   type,              // IN: type of reserved data
        void        *buffer             // IN: data buffer
                     );

#endif // __TPM2_NV_FP_H
