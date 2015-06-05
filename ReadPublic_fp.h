/*
 * Copyright 2015 The Chromium OS Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef __TPM2_READPUBLIC_FP_H_
#define __TPM2_READPUBLIC_FP_H_

typedef struct { TPMI_DH_OBJECT objectHandle; } ReadPublic_In;

typedef struct {
  TPM2B_PUBLIC outPublic;
  TPM2B_NAME name;
  TPM2B_NAME qualifiedName;
} ReadPublic_Out;

TPM_RC TPM2_ReadPublic(ReadPublic_In *in,   // IN: input parameter list
                       ReadPublic_Out *out  // OUT: output parameter list
                       );

#endif  // __TPM2_READPUBLIC_FP_H_
