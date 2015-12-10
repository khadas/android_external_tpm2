/*
 * Copyright 2015 The Chromium OS Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef __TPM2_CLEARCONTROL_FP_H
#define __TPM2_CLEARCONTROL_FP_H

typedef struct {
        TPMI_YES_NO disable;
        TPM_RH  auth;
} ClearControl_In;

TPM_RC TPM2_ClearControl(
        ClearControl_In       *in                 // IN: input parameter list
);

#endif // __TPM2_CLEARCONTROL_FP_H
