
# Copyright 2015 The Chromium OS Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

obj ?= ./build
CC ?= gcc

SOURCES += ActivateCredential.c
SOURCES += AlgorithmCap.c
SOURCES += Attest_spt.c
SOURCES += Bits.c
SOURCES += Cancel.c
SOURCES += Certify.c
SOURCES += CertifyCreation.c
SOURCES += ChangeEPS.c
SOURCES += ChangePPS.c
SOURCES += Clear.c
SOURCES += ClearControl.c
SOURCES += Clock.c
SOURCES += ClockRateAdjust.c
SOURCES += ClockSet.c
SOURCES += CommandAudit.c
SOURCES += CommandCodeAttributes.c
SOURCES += Commit.c
SOURCES += ContextLoad.c
SOURCES += ContextSave.c
SOURCES += Context_spt.c
SOURCES += CpriCryptPri.c
SOURCES += CpriECC.c
SOURCES += CpriHash.c
SOURCES += CpriMisc.c
SOURCES += CpriRNG.c
SOURCES += CpriRSA.c
SOURCES += CpriSym.c
SOURCES += Create.c
SOURCES += CreatePrimary.c
SOURCES += CryptSelfTest.c
SOURCES += CryptUtil.c
SOURCES += DA.c
SOURCES += DRTM.c
SOURCES += DictionaryAttackLockReset.c
SOURCES += DictionaryAttackParameters.c
SOURCES += Duplicate.c
SOURCES += ECC_Parameters.c
SOURCES += ECDH_KeyGen.c
SOURCES += ECDH_ZGen.c
SOURCES += EC_Ephemeral.c
SOURCES += EncryptDecrypt.c
SOURCES += Entity.c
SOURCES += Entropy.c
SOURCES += EventSequenceComplete.c
SOURCES += EvictControl.c
SOURCES += FieldUpgradeData.c
SOURCES += FieldUpgradeStart.c
SOURCES += FirmwareRead.c
SOURCES += FlushContext.c
SOURCES += GetCapability.c
SOURCES += GetCommandAuditDigest.c
SOURCES += GetRandom.c
SOURCES += GetSessionAuditDigest.c
SOURCES += GetTestResult.c
SOURCES += GetTime.c
SOURCES += Global.c
SOURCES += HMAC.c
SOURCES += HMAC_Start.c
SOURCES += Handle.c
SOURCES += Hash.c
SOURCES += HashSequenceStart.c
SOURCES += Hierarchy.c
SOURCES += HierarchyChangeAuth.c
SOURCES += HierarchyControl.c
SOURCES += Import.c
SOURCES += IncrementalSelfTest.c
SOURCES += Load.c
SOURCES += LoadExternal.c
SOURCES += Locality.c
SOURCES += LocalityPlat.c
SOURCES += MakeCredential.c
SOURCES += Manufacture.c
SOURCES += Marshal.c
SOURCES += MathFunctions.c
SOURCES += MemoryLib.c
SOURCES += NV.c
SOURCES += NVMem.c
SOURCES += NV_Certify.c
SOURCES += NV_ChangeAuth.c
SOURCES += NV_DefineSpace.c
SOURCES += NV_Extend.c
SOURCES += NV_GlobalWriteLock.c
SOURCES += NV_Increment.c
SOURCES += NV_Read.c
SOURCES += NV_ReadLock.c
SOURCES += NV_ReadPublic.c
SOURCES += NV_SetBits.c
SOURCES += NV_UndefineSpace.c
SOURCES += NV_UndefineSpaceSpecial.c
SOURCES += NV_Write.c
SOURCES += NV_WriteLock.c
SOURCES += NV_spt.c
SOURCES += Object.c
SOURCES += ObjectChangeAuth.c
SOURCES += Object_spt.c
SOURCES += PCR.c
SOURCES += PCR_Allocate.c
SOURCES += PCR_Event.c
SOURCES += PCR_Extend.c
SOURCES += PCR_Read.c
SOURCES += PCR_Reset.c
SOURCES += PCR_SetAuthPolicy.c
SOURCES += PCR_SetAuthValue.c
SOURCES += PP.c
SOURCES += PPPlat.c
SOURCES += PP_Commands.c
SOURCES += PlatformData.c
SOURCES += PolicyAuthValue.c
SOURCES += PolicyAuthorize.c
SOURCES += PolicyCommandCode.c
SOURCES += PolicyCounterTimer.c
SOURCES += PolicyCpHash.c
SOURCES += PolicyDuplicationSelect.c
SOURCES += PolicyGetDigest.c
SOURCES += PolicyLocality.c
SOURCES += PolicyNV.c
SOURCES += PolicyNameHash.c
SOURCES += PolicyNvWritten.c
SOURCES += PolicyOR.c
SOURCES += PolicyPCR.c
SOURCES += PolicyPassword.c
SOURCES += PolicyPhysicalPresence.c
SOURCES += PolicyRestart.c
SOURCES += PolicySecret.c
SOURCES += PolicySigned.c
SOURCES += PolicyTicket.c
SOURCES += Policy_spt.c
SOURCES += Power.c
SOURCES += PowerPlat.c
SOURCES += PropertyCap.c
SOURCES += Quote.c
SOURCES += RSAData.c
SOURCES += RSAKeySieve.c
SOURCES += RSA_Decrypt.c
SOURCES += RSA_Encrypt.c
SOURCES += ReadClock.c
SOURCES += ReadPublic.c
SOURCES += Rewrap.c
SOURCES += SelfTest.c
SOURCES += SequenceComplete.c
SOURCES += SequenceUpdate.c
SOURCES += Session.c
SOURCES += SessionProcess.c
SOURCES += SetAlgorithmSet.c
SOURCES += SetCommandCodeAuditStatus.c
SOURCES += SetPrimaryPolicy.c
SOURCES += Shutdown.c
SOURCES += Sign.c
SOURCES += StartAuthSession.c
SOURCES += Startup.c
SOURCES += StirRandom.c
SOURCES += TPMCmdp.c
SOURCES += TPMCmds.c
SOURCES += TcpServer.c
SOURCES += TestParms.c
SOURCES += Ticket.c
SOURCES += Time.c
SOURCES += TpmFail.c
SOURCES += Unique.c
SOURCES += Unseal.c
SOURCES += VerifySignature.c
SOURCES += ZGen_2Phase.c
SOURCES += _TPM_Hash_Data.c
SOURCES += _TPM_Hash_End.c
SOURCES += _TPM_Hash_Start.c
SOURCES += _TPM_Init.c

OBJS = $(patsubst %.c,$(obj)/%.o,$(SOURCES))
DEPS = $(patsubst %.c,$(obj)/.%.d,$(SOURCES))

$(obj)/%.o: %.c
	$(CC) -Wall -Werror -c -o $@ $<

$(obj)/.%.d: %.c
	$(CC) -M $<  > $@.tmp && mv $@.tmp $@
	sed -i "s|^\([a-zA-Z0-9_]\+\.o:\)|$(obj)/\1 $@ |"  $@

$(obj)/tpm2lib: $(obj) $(OBJS)
	ar $@ $^


$(obj):
	[ -d $(obj) ] || mkdir $(obj)

.PHONY: clean
clean:
	\rm -f ${OBJS} ${DEPS} $(obj)/tpm2lib

-include $(DEPS)
