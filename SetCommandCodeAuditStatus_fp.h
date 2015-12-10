/*
 * Copyright 2015 The Chromium OS Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef __TPM2_SETCOMMANDCODEAUDITSTATUS_FP_H_
#define __TPM2_SETCOMMANDCODEAUDITSTATUS_FP_H_

typedef struct {
        TPML_CC     clearList;
        TPML_CC     setList;
        TPM_ALG_ID  auditAlg;
} SetCommandCodeAuditStatus_In;

TPM_RC TPM2_SetCommandCodeAuditStatus(
        SetCommandCodeAuditStatus_In      *in             // IN: input parameter list
);

#endif // __TPM2_SETCOMMANDCODEAUDITSTATUS_FP_H_
