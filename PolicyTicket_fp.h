/*
 * Copyright 2015 The Chromium OS Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef __TPM2_POLICYTICKET_FP_H_
#define __TPM2_POLICYTICKET_FP_H_

typedef struct {
        TPM_HANDLE      policySession;
        TPM2B_DATA      timeout;
        TPM2B_DIGEST    cpHashA;
        TPMT_TK_COMMON  ticket;
        TPM2B_NONCE     policyRef;
        TPM2B_NAME      authName;
} PolicyTicket_In;

TPM_RC TPM2_PolicyTicket(
        PolicyTicket_In    *in                   // IN: input parameter list
);

#endif // __TPM2_POLICYTICKET_FP_H_
