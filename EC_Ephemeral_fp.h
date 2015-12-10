/*
 * Copyright 2015 The Chromium OS Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef __TPM2_EC_EPHEMERAL_FP_H
#define __TPM2_EC_EPHEMERAL_FP_H

typedef struct { TPM_ECC_CURVE curveID; } EC_Ephemeral_In;

typedef struct {
  UINT16 counter;
  TPM2B_ECC_POINT Q;
} EC_Ephemeral_Out;

TPM_RC TPM2_EC_Ephemeral(EC_Ephemeral_In *in,   // IN: input parameter list
                         EC_Ephemeral_Out *out  // OUT: output parameter list
                         );

#endif  // __TPM2_EC_EPHEMERAL_FP_H
