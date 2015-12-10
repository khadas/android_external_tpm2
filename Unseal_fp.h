/*
 * Copyright 2015 The Chromium OS Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef __TPM2_UNSEAL_FP_H_
#define __TPM2_UNSEAL_FP_H_

typedef struct { TPMI_DH_OBJECT itemHandle; } Unseal_In;

typedef struct { TPM2B_DATA outData; } Unseal_Out;

TPM_RC TPM2_Unseal(Unseal_In *in, Unseal_Out *out);

#endif  // __TPM2_UNSEAL_FP_H_
