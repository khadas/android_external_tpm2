/*
 * Copyright 2015 The Chromium OS Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef __TPM2_HIERARCHYCHANGEAUTH_FP_H
#define __TPM2_HIERARCHYCHANGEAUTH_FP_H

typedef struct {
  TPM_RH authHandle;
  TPM2B_AUTH newAuth;
} HierarchyChangeAuth_In;

TPM_RC TPM2_HierarchyChangeAuth(
    HierarchyChangeAuth_In *in  // IN: input parameter list
    );

#endif  // __TPM2_HIERARCHYCHANGEAUTH_FP_H
