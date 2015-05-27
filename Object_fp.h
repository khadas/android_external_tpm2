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

#endif // __TPM2_OBJECT_FP_H
