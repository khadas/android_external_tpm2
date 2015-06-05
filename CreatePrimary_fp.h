/*
 * Copyright 2015 The Chromium OS Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef __TPM2_CREATEPRIMARY_FP_H
#define __TPM2_CREATEPRIMARY_FP_H

typedef struct {
  TPM2B_PUBLIC inPublic;
  TPMI_DH_OBJECT primaryHandle;
  TPML_PCR_SELECTION creationPCR;
  TPM2B_DATA outsideInfo;
  TPM2B_SENSITIVE_CREATE inSensitive;
} CreatePrimary_In;

typedef struct {
  TPM2B_PUBLIC outPublic;
  TPM2B_CREATION_DATA creationData;
  TPM2B_DIGEST creationHash;
  TPMT_TK_CREATION creationTicket;
  TPMI_DH_OBJECT objectHandle;
  TPM2B_NAME name;
} CreatePrimary_Out;

TPM_RC TPM2_CreatePrimary(CreatePrimary_In *in,   // IN: input parameter list
                          CreatePrimary_Out *out  // OUT: output parameter list
                          );

#endif  // __TPM2_CREATEPRIMARY_FP_H
