/*
 * Copyright 2015 The Chromium OS Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef __TPM2_GETCAPABILITY_FP_H
#define __TPM2_GETCAPABILITY_FP_H

typedef struct {
  TPM_CAP capability;
  UINT32 propertyCount;
  UINT32 property;
} GetCapability_In;

typedef struct {
  TPMI_YES_NO moreData;
  TPMS_CAPABILITY_DATA capabilityData;
} GetCapability_Out;

TPM_RC TPM2_GetCapability(GetCapability_In *in,   // IN: input parameter list
                          GetCapability_Out *out  // OUT: output parameter list
                          );

#endif  // __TPM2_GETCAPABILITY_FP_H
