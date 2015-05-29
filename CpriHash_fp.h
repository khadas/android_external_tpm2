/*
 * Copyright 2015 The Chromium OS Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef __TPM2_CPRIHASH_FP_H
#define __TPM2_CPRIHASH_FP_H

LIB_EXPORT UINT16 _cpri__CompleteHash(
        CPRI_HASH_STATE         *hashState,             // IN: the state of hash stack
        UINT32                   dOutSize,              // IN: size of digest buffer
        BYTE                    *dOut                   // OUT: hash digest
);
LIB_EXPORT BOOL _cpri__HashStartup(void);
LIB_EXPORT UINT16 _cpri__GetDigestSize(
        TPM_ALG_ID           hashAlg               // IN: hash algorithm to look up
);
LIB_EXPORT UINT16 _cpri__KDFa(
        TPM_ALG_ID          hashAlg,             //   IN: hash algorithm used in HMAC
        TPM2B              *key,                 //   IN: HMAC key
        const char         *label,               //   IN: a 0-byte terminated label used in KDF
        TPM2B              *contextU,            //   IN: context U
        TPM2B              *contextV,            //   IN: context V
        UINT32              sizeInBits,          //   IN: size of generated key in bit
        BYTE               *keyStream,           //   OUT: key buffer
        UINT32             *counterInOut,        //   IN/OUT: caller may provide the iteration
                                             //       counter for incremental operations to
                                             //       avoid large intermediate buffers.
        BOOL                once                 //   IN: TRUE if only one iteration is performed
                                             //       FALSE if iteration count determined by
                                             //       "sizeInBits"
);
LIB_EXPORT UINT16 _cpri__StartHash(
        TPM_ALG_ID               hashAlg,              // IN: hash algorithm
        BOOL                     sequence,             // IN: TRUE if the state should be saved
        CPRI_HASH_STATE         *hashState             // OUT: the state of hash stack.
);
LIB_EXPORT void _cpri__UpdateHash(
        CPRI_HASH_STATE           *hashState,      // IN: the hash context information
        UINT32                     dataSize,       // IN: the size of data to be added to the
                                              //     digest
        BYTE                      *data            // IN: data to be hashed
);

#endif // __TPM2_CPRIHASH_FP_H
