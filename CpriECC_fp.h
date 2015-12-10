/*
 * Copyright 2015 The Chromium OS Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef __TPM2_CPRIECC_FP_H
#define __TPM2_CPRIECC_FP_H

LIB_EXPORT BOOL _cpri__EccIsPointOnCurve(
        TPM_ECC_CURVE          curveId,             // IN: the curve selector
        TPMS_ECC_POINT        *Q                    // IN: the point.
                                         );
LIB_EXPORT BOOL _cpri__EccStartup(void);

#endif // __TPM2_CPRIECC_FP_H
