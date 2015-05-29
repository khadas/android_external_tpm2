/*
 * Copyright 2015 The Chromium OS Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef __TPM2_CPRISYM_FP_H
#define __TPM2_CPRISYM_FP_H

//
//       _cpri__AESDecryptCTR()
//
// Counter mode decryption uses the same algorithm as encryption. The
// _cpri__AESDecryptCTR() function is implemented as a macro call to
// _cpri__AESEncryptCTR().
#define _cpri__AESDecryptCTR(dOut, keySize, key, iv, dInSize, dIn) \
        _cpri__AESEncryptCTR(                                      \
                ((BYTE *)dOut),                                    \
                ((UINT32)keySize),                                 \
                ((BYTE *)key),                                     \
                ((BYTE *)iv),                                      \
                ((UINT32)dInSize),                                 \
                ((BYTE *)dIn)                                      \
        )

//
//       _cpri__AESDecryptOFB()
//
// OFB encryption and decryption use the same algorithms for both. The
// _cpri__AESDecryptOFB() function is implemented as a macro call to
// _cpri__AESEncrytOFB().
//
#define _cpri__AESDecryptOFB(dOut,keySizeInBits, key, iv, dInSize, dIn) \
        _cpri__AESEncryptOFB (                                          \
                ((BYTE *)dOut),                                         \
                ((UINT32)keySizeInBits),                                \
                ((BYTE *)key),                                          \
                ((BYTE *)iv),                                           \
                ((UINT32)dInSize),                                      \
                ((BYTE *)dIn)                                           \
        )

//
//    _cpri__SM4DecryptCTR()
//
// Counter mode decryption uses the same algorithm as encryption. The
// _cpri__SM4DecryptCTR() function is implemented as a macro call to
// _cpri__SM4EncryptCTR().
//
#define _cpri__SM4DecryptCTR(dOut, keySize, key, iv, dInSize, dIn)      \
        _cpri__SM4EncryptCTR(                                           \
                ((BYTE *)dOut),                                         \
                ((UINT32)keySize),                                      \
                ((BYTE *)key),                                          \
                ((BYTE *)iv),                                           \
                ((UINT32)dInSize),                                      \
                ((BYTE *)dIn)                                           \
        )

//
//       _cpri__SM4DecryptOFB()
//
// OFB encryption and decryption use the same algorithms for both. The
// _cpri__SM4DecryptOFB() function is implemented as a macro call to
// _cpri__SM4EncrytOFB().
//
#define _cpri__SM4DecryptOFB(dOut,keySizeInBits, key, iv, dInSize, dIn) \
        _cpri__SM4EncryptOFB (                                          \
                ((BYTE *)dOut),                                         \
                ((UINT32)keySizeInBits),                                \
                ((BYTE *)key),                                          \
                ((BYTE *)iv),                                           \
                ((UINT32)dInSize),                                      \
                ((BYTE *)dIn)                                           \
        )


LIB_EXPORT BOOL _cpri__SymStartup(void);

#endif // __TPM2_CPRISYM_FP_H
