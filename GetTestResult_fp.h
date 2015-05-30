/*
 * Copyright 2015 The Chromium OS Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef __TPM2_GETTESTRESULT_FP_H
#define __TPM2_GETTESTRESULT_FP_H

typedef struct {
        TPM_RC testResult;
        TPM2B_MAX_BUFFER outData;
} GetTestResult_Out;

TPM_RC TPM2_GetTestResult(
        GetTestResult_Out      *out               // OUT: output parameter list
                          );

#endif // __TPM2_GETTESTRESULT_FP_H
