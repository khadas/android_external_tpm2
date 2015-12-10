/*
 * Copyright 2015 The Chromium OS Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef __TPM2_TICKET_FP_H
#define __TPM2_TICKET_FP_H

void TicketComputeCreation(
        TPMI_RH_HIERARCHY       hierarchy,        //   IN: hierarchy for ticket
        TPM2B_NAME             *name,             //   IN: object name
        TPM2B_DIGEST           *creation,         //   IN: creation hash
        TPMT_TK_CREATION       *ticket            //   OUT: created ticket
);

#endif // __TPM2_TICKET_FP_H
