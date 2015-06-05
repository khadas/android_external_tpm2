/*
 * Copyright 2015 The Chromium OS Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef __TPM2_CONTEXTSAVE_FP_H
#define __TPM2_CONTEXTSAVE_FP_H

typedef struct { TPMI_DH_OBJECT saveHandle; } ContextSave_In;

typedef struct { TPMS_CONTEXT context; } ContextSave_Out;

TPM_RC TPM2_ContextSave(ContextSave_In *in,   // IN: input parameter list
                        ContextSave_Out *out  // OUT: output parameter list
                        );

#endif  // __TPM2_CONTEXTSAVE_FP_H
