/*
 * Copyright 2015 The Chromium OS Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef __TPM2_OBJECT_SPT_FP_H
#define __TPM2_OBJECT_SPT_FP_H

TPM_RC CredentialToSecret(
        TPM2B_ID_OBJECT          *inIDObject,             //   IN: input credential blob
        TPM2B_NAME               *name,                   //   IN: the name of the object
        TPM2B_SEED               *seed,                   //   IN: an external seed.
        TPM_HANDLE                protector,              //   IN: The protector's handle
        TPM2B_DIGEST             *secret                  //   OUT: secret information
);

#endif // __TPM2_OBJECT_SPT_FP_H
