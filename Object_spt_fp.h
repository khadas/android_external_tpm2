/*
 * Copyright 2015 The Chromium OS Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef __TPM2_OBJECT_SPT_FP_H
#define __TPM2_OBJECT_SPT_FP_H

BOOL AreAttributesForParent(
        OBJECT             *parentObject        // IN: parent handle
);
TPM_RC CredentialToSecret(
        TPM2B_ID_OBJECT          *inIDObject,             //   IN: input credential blob
        TPM2B_NAME               *name,                   //   IN: the name of the object
        TPM2B_SEED               *seed,                   //   IN: an external seed.
        TPM_HANDLE                protector,              //   IN: The protector's handle
        TPM2B_DIGEST             *secret                  //   OUT: secret information
);
void FillInCreationData(
        TPMI_DH_OBJECT                     parentHandle,    //   IN: handle of parent
        TPMI_ALG_HASH                      nameHashAlg,     //   IN: name hash algorithm
        TPML_PCR_SELECTION                *creationPCR,     //   IN: PCR selection
        TPM2B_DATA                        *outsideData,     //   IN: outside data
        TPM2B_CREATION_DATA               *outCreation,     //   OUT: creation data for output
        TPM2B_DIGEST                      *creationDigest   //   OUT: creation digest
);
TPM_RC PublicAttributesValidation(
        BOOL                load,                 // IN: TRUE if load checks, FALSE if
                                             //     TPM2_Create()
        TPMI_DH_OBJECT      parentHandle,         // IN: input parent handle
        TPMT_PUBLIC        *publicArea            // IN: public area of the object
);
void SensitiveToPrivate(
        TPMT_SENSITIVE      *sensitive,         //   IN: sensitive structure
        TPM2B_NAME          *name,              //   IN: the name of the object
        TPM_HANDLE           parentHandle,      //   IN: The parent's handle
        TPM_ALG_ID           nameAlg,           //   IN: hash algorithm in public area. This
                                            //       parameter is used when parentHandle is
                                            //       NULL, in which case the object is
                                            //       temporary.
        TPM2B_PRIVATE       *outPrivate         //   OUT: output private structure
);

#endif // __TPM2_OBJECT_SPT_FP_H
