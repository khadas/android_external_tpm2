// This file was extracted from the TCG Published
// Trusted Platform Module Library
// Part 4: Supporting Routines
// Family "2.0"
// Level 00 Revision 01.16
// October 30, 2014

#ifndef _PLATFORM_DATA_H_
#define _PLATFORM_DATA_H_
#include    "TpmBuildSwitches.h"
#include    "Implementation.h"
#include    "bool.h"
//
//     From Cancel.c Cancel flag. It is initialized as FALSE, which indicate the command is not being canceled
//
extern BOOL         s_isCanceled;
//
//     From Clock.c This variable records the time when _plat__ClockReset() is called. This mechanism allow
//     us to subtract the time when TPM is power off from the total time reported by clock() function
//
extern unsigned long long          s_initClock;
extern unsigned int                s_adjustRate;
//
//     From LocalityPlat.c Locality of current command
//
extern unsigned char s_locality;
//
//     From NVMem.c Choose if the NV memory should be backed by RAM or by file. If this macro is defined,
//     then a file is used as NV. If it is not defined, then RAM is used to back NV memory. Comment out to use
//     RAM.
//
#define FILE_BACKED_NV
#if defined FILE_BACKED_NV
#include <stdio.h>
//
//     A file to emulate NV storage
//
extern   FILE*                  s_NVFile;
#endif
extern   unsigned char          s_NV[NV_MEMORY_SIZE];
extern   BOOL                   s_NvIsAvailable;
extern   BOOL                   s_NV_unrecoverable;
extern   BOOL                   s_NV_recoverable;
//
//     From PPPlat.c Physical presence. It is initialized to FALSE
//
extern BOOL         s_physicalPresence;
//
//     From Power
//
extern BOOL             s_powerLost;
//
//     From Entropy.c
//
extern uint32_t        lastEntropy;
extern int             firstValue;
#endif // _PLATFORM_DATA_H_
