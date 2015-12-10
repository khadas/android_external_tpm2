/*
 * Copyright 2015 The Chromium OS Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef __TPM2_NV_CHANGEAUTH_FP_H
#define __TPM2_NV_CHANGEAUTH_FP_H

typedef struct {
  TPMI_RH_NV_INDEX nvIndex;
  TPM2B_AUTH newAuth;
} NV_ChangeAuth_In;

TPM_RC TPM2_NV_ChangeAuth(NV_ChangeAuth_In *in);

#endif  // __TPM2_NV_CHANGEAUTH_FP_H
