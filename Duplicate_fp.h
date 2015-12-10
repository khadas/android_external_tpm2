/*
 * Copyright 2015 The Chromium OS Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef __TPM2_DUPLICATE_FP_H
#define __TPM2_DUPLICATE_FP_H
typedef struct {
  TPMI_DH_OBJECT objectHandle;
  TPMI_DH_OBJECT newParentHandle;
  TPMT_SYM_DEF_OBJECT symmetricAlg;
  TPM2B_DATA encryptionKeyIn;
} Duplicate_In;

typedef struct {
  TPM2B_PRIVATE duplicate;
  TPM2B_ENCRYPTED_SECRET outSymSeed;
  TPM2B_DATA encryptionKeyOut;
} Duplicate_Out;

TPM_RC TPM2_Duplicate(Duplicate_In *in,   // IN: input parameter list
                      Duplicate_Out *out  // OUT: output parameter list
                      );

#endif  // __TPM2_DUPLICATE_FP_H
