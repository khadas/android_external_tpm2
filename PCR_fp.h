/*
 * Copyright 2015 The Chromium OS Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef __TPM2_PCR_FP_H
#define __TPM2_PCR_FP_H

TPMI_ALG_HASH PCRGetAuthPolicy(
        TPMI_DH_PCR          handle,            // IN: PCR handle
        TPM2B_DIGEST        *policy             // OUT: policy of PCR
                               );
void PCRGetAuthValue(
        TPMI_DH_PCR          handle,            // IN: PCR handle
        TPM2B_AUTH          *auth               // OUT: authValue of PCR
                     );
void PCRExtend(
        TPMI_DH_PCR          handle,              //   IN:    PCR handle to be extended
        TPMI_ALG_HASH        hash,                //   IN:    hash algorithm of PCR
        UINT32               size,                //   IN:    size of data to be extended
        BYTE                *data                 //   IN:    data to be extended
               );
BOOL PCRIsExtendAllowed(
        TPMI_DH_PCR          handle               // IN: PCR handle to be extended
                        );
BOOL PCRIsStateSaved(
        TPMI_DH_PCR         handle                // IN: PCR handle to be extended
                     );

#endif // __TPM2_PCR_FP_H
