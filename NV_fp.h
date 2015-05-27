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
void NvGetIndexInfo(
        TPMI_RH_NV_INDEX          handle,              // IN: handle
        NV_INDEX                 *nvIndex              // OUT: NV index structure
                    );
UINT16 NvGetName(
        TPMI_RH_NV_INDEX          handle,            // IN: handle of the index
        NAME                     *name               // OUT: name of the index
                 );
TPM_RC NvIndexIsAccessible(
        TPMI_RH_NV_INDEX         handle,                // IN: handle
        TPM_CC                   commandCode            // IN: the command
                           );
TPM_RC NvIsAvailable(
        void
);
void NvWriteReserved(
        NV_RESERVE   type,              // IN: type of reserved data
        void        *buffer             // IN: data buffer
                     );

#endif // __TPM2_NV_FP_H
