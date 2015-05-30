/*
 * Copyright 2015 The Chromium OS Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef __TPM2_ECC_PARAMETERS_FP_H
#define __TPM2_ECC_PARAMETERS_FP_H

typedef struct {
        TPM_ECC_CURVE   curveID;
} ECC_Parameters_In;

typedef struct {
        TPMS_ALGORITHM_DETAIL_ECC       parameters;
} ECC_Parameters_Out;

TPM_RC TPM2_ECC_Parameters(
        ECC_Parameters_In     *in,                // IN: input parameter list
        ECC_Parameters_Out    *out                // OUT: output parameter list
                           );

#endif // __TPM2_ECC_PARAMETERS_FP_H
