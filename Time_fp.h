/*
 * Copyright 2015 The Chromium OS Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef __TPM2_TIME_FP_H
#define __TPM2_TIME_FP_H

void TimeFillInfo(
        TPMS_CLOCK_INFO           *clockInfo
);
void TimeSetAdjustRate(
        TPM_CLOCK_ADJUST          adjust            // IN: adjust constant
);

#endif // __TPM2_TIME_FP_H
