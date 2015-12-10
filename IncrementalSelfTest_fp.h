/*
 * Copyright 2015 The Chromium OS Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef __TPM2_INCREMENTALSELFTEST_FP_H
#define __TPM2_INCREMENTALSELFTEST_FP_H

typedef struct { TPML_ALG toTest; } IncrementalSelfTest_In;

typedef struct { TPML_ALG toDoList; } IncrementalSelfTest_Out;

TPM_RC TPM2_IncrementalSelfTest(
    IncrementalSelfTest_In *in,   // IN: input parameter list
    IncrementalSelfTest_Out *out  // OUT: output parameter list
    );

#endif  // __TPM2_INCREMENTALSELFTEST_FP_H
