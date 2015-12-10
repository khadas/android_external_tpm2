/*
 * Copyright 2015 The Chromium OS Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef __TPM2_ECDH_ZGEN_FP_H
#define __TPM2_ECDH_ZGEN_FP_H

typedef struct {
        TPMI_DH_OBJECT  keyHandle;
        TPM2B_ECC_POINT inPoint;
} ECDH_ZGen_In;

typedef struct {
        TPM2B_ECC_POINT outPoint;
} ECDH_ZGen_Out;

TPM_RC TPM2_ECDH_ZGen(
        ECDH_ZGen_In      *in,                  // IN: input parameter list
        ECDH_ZGen_Out     *out                  // OUT: output parameter list
                      );

#endif // __TPM2_ECDH_ZGEN_FP_H
