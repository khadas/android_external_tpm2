/*
 * Copyright 2015 The Chromium OS Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef __TPM2_READCLOCK_FP_H_
#define __TPM2_READCLOCK_FP_H_

typedef struct {
        TPMS_TIME_INFO  currentTime;
} ReadClock_Out;

TPM_RC TPM2_ReadClock(
        ReadClock_Out    *out            // OUT: output parameter list
);

#endif // __TPM2_READCLOCK_FP_H_
