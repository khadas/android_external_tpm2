/*
 * Copyright 2015 The Chromium OS Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef __TPM2_SESSION_FP_H
#define __TPM2_SESSION_FP_H

TPM_RC SessionContextLoad(
        SESSION            *session,            // IN: session structure from saved context
        TPM_HANDLE         *handle              // IN/OUT: session handle
                          );
TPM_RC SessionContextSave (
        TPM_HANDLE                 handle,           // IN: session handle
        CONTEXT_COUNTER           *contextID         // OUT: assigned contextID
                           );
SESSION *SessionGet(
        TPM_HANDLE           handle              // IN: session handle
);
BOOL SessionIsLoaded(
        TPM_HANDLE             handle                // IN: session handle
                     );
BOOL SessionIsSaved(
        TPM_HANDLE            handle                // IN: session handle
                    );

#endif // __TPM2_SESSION_FP_H
