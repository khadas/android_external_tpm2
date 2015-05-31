/*
 * Copyright 2015 The Chromium OS Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef __SOURCE_ENTITY_FP_H
#define __SOURCE_ENTITY_FP_H

TPMI_RH_HIERARCHY EntityGetHierarchy(
        TPMI_DH_ENTITY       handle             // IN :handle of entity
);

#endif // __SOURCE_ENTITY_FP_H
TPM_RC EntityGetLoadStatus(
        TPM_HANDLE          *handle,              // IN/OUT: handle of the entity
        TPM_CC               commandCode          // IN: the commmandCode
        );
