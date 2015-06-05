/*
 * Copyright 2015 The Chromium OS Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef __TPM2_ECDH_KEYGEN_FP_H
#define __TPM2_ECDH_KEYGEN_FP_H

typedef struct { TPMI_DH_OBJECT keyHandle; } ECDH_KeyGen_In;

typedef struct {
  TPM2B_ECC_POINT pubPoint;
  TPM2B_ECC_POINT zPoint;
} ECDH_KeyGen_Out;

TPM_RC TPM2_ECDH_KeyGen(ECDH_KeyGen_In *in,   // IN: input parameter list
                        ECDH_KeyGen_Out *out  // OUT: output parameter list
                        );

#endif  // __TPM2_ECDH_KEYGEN_FP_H
