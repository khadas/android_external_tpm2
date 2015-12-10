// This file was extracted from the TCG Published
// Trusted Platform Module Library
// Part 4: Supporting Routines
// Family "2.0"
// Level 00 Revision 01.16
// October 30, 2014

#include        "Implementation.h"
#include        "Platform.h"
#include        "PlatformData.h"
//
//     From Cancel.c
//
BOOL                      s_isCanceled;
//
//     From Clock.c
//
unsigned long long        s_initClock;
unsigned int              s_adjustRate;
//
//     From LocalityPlat.c
//
unsigned char             s_locality;
//
//     From Power.c
//
BOOL                      s_powerLost;
//
//     From Entropy.c
//
uint32_t                  lastEntropy;
int                       firstValue;
//
//     From NVMem.c
//
#ifdef VTPM
#    undef FILE_BACKED_NV
#endif
#ifdef FILE_BACKED_NV
FILE                 *s_NVFile = NULL;
#endif
unsigned char         s_NV[NV_MEMORY_SIZE];
BOOL                  s_NvIsAvailable;
BOOL                  s_NV_unrecoverable;
BOOL                  s_NV_recoverable;
//
//     From PPPlat.c
//
BOOL   s_physicalPresence;
