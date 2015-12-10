/*
 * Copyright 2015 The Chromium OS Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef __TPM2_HASH_FP_H
#define __TPM2_HASH_FP_H

typedef struct {
  TPMI_ALG_HASH hashAlg;
  TPMI_RH_HIERARCHY hierarchy;
  TPM2B_DATA data;
} Hash_In;

typedef struct {
  TPM2B_DIGEST outHash;
  TPMT_TK_HASHCHECK validation;
} Hash_Out;

TPM_RC TPM2_Hash(Hash_In *in,   // IN: input parameter list
                 Hash_Out *out  // OUT: output parameter list
                 );

#endif  // __TPM2_HASH_FP_H
