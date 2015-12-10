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
UINT16 CryptCommit(void);
TPM_RC CryptCommitCompute(
        TPMS_ECC_POINT                *K,                     //   OUT: [d]B
        TPMS_ECC_POINT                *L,                     //   OUT: [r]B
        TPMS_ECC_POINT                *E,                     //   OUT: [r]M
        TPM_ECC_CURVE                  curveID,               //   IN: The curve for the computation
        TPMS_ECC_POINT                *M,                     //   IN: M (P1)
        TPMS_ECC_POINT                *B,                     //   IN: B (x2, y2)
        TPM2B_ECC_PARAMETER           *d,                     //   IN: the private scalar
        TPM2B_ECC_PARAMETER           *r                      //   IN: the computed r value
                          );
UINT16 CryptCompleteHash2B(
        void      *state,       // IN: the state of hash stack
        TPM2B     *digest       // IN: the size of the buffer Out: requested
                                //     number of byte
);
void CryptDrbgGetPutState(
        GET_PUT              direction         // IN: Get from or put to DRBG
);
TPM_RC CryptDecryptRSA(
        UINT16                    *dataOutSize,       // OUT: size of plain text in byte
        BYTE                    *dataOut,        //   OUT: plain text
        OBJECT                  *rsaKey,         //   IN: internal RSA key
        TPMT_RSA_DECRYPT        *scheme,         //   IN: selects the padding scheme
        UINT16                   cipherInSize,   //   IN: size of cipher text in byte
        BYTE                    *cipherIn,       //   IN: cipher text
        const char              *label           //   IN: a label, when needed
);
TPM_RC CryptDivide(
        TPM2B               *numerator,           //   IN: numerator
        TPM2B               *denominator,         //   IN: denominator
        TPM2B               *quotient,            //   OUT: quotient = numerator / denominator.
        TPM2B               *remainder            //   OUT: numerator mod denominator.
                   );
void CryptDrbgGetPutState(
        GET_PUT              direction         // IN: Get from or put to DRBG
);
LIB_EXPORT const TPM2B * CryptEccGetParameter(
        char                 p,                  // IN: the parameter selector
        TPM_ECC_CURVE        curveId             // IN: the curve id
                                              );
BOOL CryptEccIsPointOnCurve(
        TPM_ECC_CURVE        curveID,            // IN: ECC curve ID
        TPMS_ECC_POINT      *Q                   // IN: ECC point
                            );
BOOL CryptGenerateR(
        TPM2B_ECC_PARAMETER           *r,                 //   OUT: the generated random value
        UINT16                        *c,                 //   IN/OUT: count value.
        TPMI_ECC_CURVE                 curveID,           //   IN: the curve for the value
        TPM2B_NAME                    *name               //   IN: optional name of a key to
                                                     //       associate with 'r'
);
UINT16 CryptGenerateRandom(
        UINT16               randomSize,       // IN: size of random number
        BYTE                *buffer            // OUT: buffer of random number
);
LIB_EXPORT UINT16 CryptGetHashDigestSize(
        TPM_ALG_ID           hashAlg              // IN: hash algorithm
                                         );
LIB_EXPORT UINT16 CryptHashBlock(
        TPM_ALG_ID          algId,               //   IN: the hash algorithm to use
        UINT16              blockSize,           //   IN: size of the data block
        BYTE               *block,               //   IN: address of the block to hash
        UINT16              retSize,             //   IN: size of the return buffer
        BYTE               *ret                  //   OUT: address of the buffer
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
void CryptSymmetricDecrypt(
        BYTE                      *decrypted,
        TPM_ALG_ID                 algorithm,       //   IN: algorithm for encryption
        UINT16                     keySizeInBits,   //   IN: key size in bit
        TPMI_ALG_SYM_MODE          mode,            //   IN: symmetric encryption mode
        BYTE                      *key,             //   IN: encryption key
        TPM2B_IV                  *ivIn,            //   IN/OUT: IV for next block
        UINT32                     dataSize,        //   IN: data size in byte
        BYTE                      *data             //   IN/OUT: data buffer
                           );
void CryptSymmetricEncrypt(
        BYTE                    *encrypted,         //   OUT: the encrypted data
        TPM_ALG_ID               algorithm,         //   IN: algorithm for encryption
        UINT16                   keySizeInBits,     //   IN: key size in bit
        TPMI_ALG_SYM_MODE        mode,              //   IN: symmetric encryption mode
        BYTE                    *key,               //   IN: encryption key
        TPM2B_IV                *ivIn,              //   IN/OUT: Input IV and output chaining
                                               //       value for the next block
        UINT32                   dataSize,          //   IN: data size in byte
        BYTE                    *data               //   IN/OUT: data buffer
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
void CryptUpdateDigestInt(
        void    *state,             // IN: the state of hash stack
        UINT32   intSize,           // IN: the size of 'intValue' in byte
        void    *intValue           // IN: integer value to be hashed
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
