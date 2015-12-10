// This file was extracted from the TCG Published
// Trusted Platform Module Library
// Part 3: Commands
// Family "2.0"
// Level 00 Revision 01.16
// October 30, 2014

#include "InternalRoutines.h"
#include "ReadClock_fp.h"
TPM_RC
TPM2_ReadClock(
   ReadClock_Out    *out            // OUT: output parameter list
   )
{
// Command Output

   out->currentTime.time = g_time;
   TimeFillInfo(&out->currentTime.clockInfo);

   return TPM_RC_SUCCESS;
}
