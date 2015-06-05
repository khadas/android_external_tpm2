/*
 * Copyright 2015 The Chromium OS Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef __TPM2_CREATE_FP_H
#define __TPM2_CREATE_FP_H

typedef struct {
  TPM2B_PUBLIC inPublic;
  TPMI_DH_OBJECT parentHandle;
  TPML_PCR_SELECTION creationPCR;
  TPM2B_DATA outsideInfo;
  TPM2B_SENSITIVE_CREATE inSensitive;
} Create_In;

typedef struct {
  TPM2B_PUBLIC outPublic;
  TPM2B_CREATION_DATA creationData;
  TPM2B_DIGEST creationHash;
  TPMT_TK_CREATION creationTicket;
  TPM2B_PRIVATE outPrivate;
} Create_Out;

TPM_RC TPM2_Create(Create_In *in,   // IN: input parameter list
                   Create_Out *out  // OUT: output parameter list
                   );

#endif  // __TPM2_CREATE_FP_H
