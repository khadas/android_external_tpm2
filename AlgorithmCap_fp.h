/*
 * Copyright 2015 The Chromium OS Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef __TPM2_ALGORITHMCAP_FP_H
#define __TPM2_ALGORITHMCAP_FP_H

void AlgorithmGetImplementedVector(
        ALGORITHM_VECTOR      *implemented            // OUT: the implemented bits are SET
        );

#endif // __TPM2_ALGORITHMCAP_FP_H
