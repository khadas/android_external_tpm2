/*
 * Copyright 2015 The Chromium OS Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef __TPM2_PCR_EVENT_FP_H_
#define __TPM2_PCR_EVENT_FP_H_

typedef struct {
  TPM_RH pcrHandle;
  TPM2B_DATA eventData;
} PCR_Event_In;

typedef struct { TPML_DIGEST digests; } PCR_Event_Out;

TPM_RC TPM2_PCR_Event(PCR_Event_In *in,   // IN: input parameter list
                      PCR_Event_Out *out  // OUT: output parameter list
                      );

#endif  // __TPM2_PCR_EVENT_FP_H_
