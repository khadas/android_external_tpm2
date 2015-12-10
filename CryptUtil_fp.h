/*
 * Copyright 2015 The Chromium OS Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef __SOURCE_CRYPTUTIL_FP_H
#define __SOURCE_CRYPTUTIL_FP_H

BOOL CryptIsAsymAlgorithm(
        TPM_ALG_ID           algID                // IN: algorithm ID
);
TPM_RC CryptSecretDecrypt(
        TPM_HANDLE      tpmKey,               // IN: decrypt key
        TPM2B_NONCE     *nonceCaller,         // IN: nonceCaller. It is needed for
                                              //     symmetric decryption. For
                                              //     asymmetric decryption, this
                                              //     parameter is NULL
        const char      *label,           // IN: a null-terminated string as L
        TPM2B_ENCRYPTED_SECRET *secret,          // IN: input secret
        TPM2B_DATA      *data             // OUT: decrypted secret value
);

#endif // __SOURCE_CRYPTUTIL_FP_H
