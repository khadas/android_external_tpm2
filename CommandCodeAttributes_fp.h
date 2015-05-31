/*
 * Copyright 2015 The Chromium OS Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef __TPM2_COMMANDCODEATTRIBUTES_FP_H
#define __TPM2_COMMANDCODEATTRIBUTES_FP_H

TPMI_YES_NO CommandCapGetCCList(
        TPM_CC            commandCode,         // IN: start command code
        UINT32            count,               // IN: maximum count for number of entries in
                                                //     'commandList'
        TPML_CCA         *commandList          // OUT: list of TPMA_CC
);
BOOL CommandIsImplemented(
        TPM_CC  commandCode          // IN: command code
                          );
BOOL IsReadOperation(
        TPM_CC               command           // IN: Command to check
        );
BOOL IsWriteOperation(
        TPM_CC               command           // IN: Command to check
        );

#endif // __TPM2_COMMANDCODEATTRIBUTES_FP_H
