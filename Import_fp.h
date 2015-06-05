/*
 * Copyright 2015 The Chromium OS Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef __TPM2_IMPORT_FP_H
#define __TPM2_IMPORT_FP_H

typedef struct {
  TPM2B_PRIVATE duplicate;
  TPMI_DH_OBJECT parentHandle;
  TPM2B_PUBLIC objectPublic;
  TPMT_SYM_DEF_OBJECT symmetricAlg;
  TPM2B_DATA encryptionKey;
  TPM2B_ENCRYPTED_SECRET inSymSeed;

} Import_In;

typedef struct { TPM2B_PRIVATE outPrivate; } Import_Out;

TPM_RC TPM2_Import(Import_In *in,   // IN: input parameter list
                   Import_Out *out  // OUT: output parameter list
                   );

#endif  // __TPM2_IMPORT_FP_H
