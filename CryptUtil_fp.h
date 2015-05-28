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
UINT16 CryptCompleteHash2B(
        void      *state,       // IN: the state of hash stack
        TPM2B     *digest       // IN: the size of the buffer Out: requested
                                //     number of byte
);
void CryptDrbgGetPutState(
        GET_PUT              direction         // IN: Get from or put to DRBG
);
UINT16 CryptGenerateRandom(
        UINT16               randomSize,       // IN: size of random number
        BYTE                *buffer            // OUT: buffer of random number
);
BOOL CryptIsSchemeAnonymous(
        TPM_ALG_ID           scheme     // IN: the scheme algorithm to test
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
TPM_RC CryptSelectSignScheme(
        TPMI_DH_OBJECT   signHandle,        // IN: handle of signing key
        TPMT_SIG_SCHEME  *scheme             // IN/OUT: signing scheme
);
TPM_RC CryptSign(
        TPMI_DH_OBJECT     signHandle,     //   IN: The handle of sign key
        TPMT_SIG_SCHEME    *signScheme,    //   IN: sign scheme.
        TPM2B_DIGEST       *digest,        //   IN: The digest being signed
        TPMT_SIGNATURE     *signature      //   OUT: signature
);
UINT16 CryptStartHash(
        TPMI_ALG_HASH        hashAlg,      // IN: hash algorithm
        HASH_STATE          *hashState     // OUT: the state of hash stack. It will be used
                                           //     in hash update and completion
);
void CryptUpdateDigest(
        void    *digestState,    // IN: the state of hash stack
        UINT32  dataSize,        // IN: the size of data
        BYTE    *data            // IN: data to be hashed
);
void KDFa(
        TPM_ALG_ID           hash,        //   IN: hash algorithm used in HMAC
        TPM2B               *key,         //   IN: HMAC key
        const char          *label,       //   IN: a null-terminated label for KDF
        TPM2B               *contextU,    //   IN: context U
        TPM2B               *contextV,    //   IN: context V
        UINT32               sizeInBits,  //   IN: size of generated key in bit
        BYTE                *keyStream,   //   OUT: key buffer
        UINT32              *counterInOut //   IN/OUT: caller may provide the iteration
                                          //       counter for incremental operations to
                                          //       avoid large intermediate buffers.
);

#endif // __SOURCE_CRYPTUTIL_FP_H
