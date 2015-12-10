/*
 * Copyright 2015 The Chromium OS Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef __TPM2_DICTIONARYATTACKPARAMETERS_FP_H
#define __TPM2_DICTIONARYATTACKPARAMETERS_FP_H

typedef struct {
  UINT32 newMaxTries;  // number of unexpired authorization failures before the
                       // TPM is in lockout
  UINT32 newRecoveryTime;  // time between authorization failures before
                           // failedTries is decremented
  UINT32 lockoutRecovery;  // time that must expire between
} DictionaryAttackParameters_In;

TPM_RC TPM2_DictionaryAttackParameters(
    DictionaryAttackParameters_In *in  // IN: input parameter list
    );

#endif  // __TPM2_DICTIONARYATTACKPARAMETERS_FP_H
