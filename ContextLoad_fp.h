/*
 * Copyright 2015 The Chromium OS Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef __TPM2_CONTEXTLOAD_FP_H
#define __TPM2_CONTEXTLOAD_FP_H

typedef struct { TPMS_CONTEXT context; } ContextLoad_In;

typedef struct { TPMI_DH_OBJECT loadedHandle; } ContextLoad_Out;

TPM_RC TPM2_ContextLoad(ContextLoad_In *in,   // IN: input parameter list
                        ContextLoad_Out *out  // OUT: output parameter list
                        );

#endif  // __TPM2_CONTEXTLOAD_FP_H
