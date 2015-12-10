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

#endif // __TPM2_PCR_FP_H
