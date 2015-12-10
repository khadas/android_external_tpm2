/*
 * Copyright 2015 The Chromium OS Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef __TPM2_CRYPTSELFTEST_FP_H
#define __TPM2_CRYPTSELFTEST_FP_H

void CryptInitializeToTest(
        void
        );
TPM_RC CryptTestAlgorithm(
        TPM_ALG_ID                 alg,
        ALGORITHM_VECTOR          *toTest
);

#endif // __TPM2_CRYPTSELFTEST_FP_H
