/*
 * Copyright 2015 The Chromium OS Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef __TPM2_OBJECT_FP_H
#define __TPM2_OBJECT_FP_H

OBJECT* ObjectGet(
        TPMI_DH_OBJECT       handle             // IN: handle of the object
);
UINT16 ObjectGetName(
        TPMI_DH_OBJECT       handle,            // IN: handle of the object
        NAME                *name               // OUT: name of the object
);
void ObjectGetQualifiedName(
        TPMI_DH_OBJECT       handle,            // IN: handle of the object
        TPM2B_NAME          *qualifiedName      // OUT: qualified name of the object
);

#endif // __TPM2_OBJECT_FP_H
