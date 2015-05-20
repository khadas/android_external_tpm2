// This file was extracted from the TCG Published
// Trusted Platform Module Library
// Part 4: Supporting Routines
// Family "2.0"
// Level 00 Revision 01.16
// October 30, 2014

#include "InternalRoutines.h"
//
//
//           LocalityGetAttributes()
//
//     This function will convert a locality expressed as an integer into TPMA_LOCALITY form.
//     The function returns the locality attribute.
//
TPMA_LOCALITY
LocalityGetAttributes(
      UINT8               locality            // IN: locality value
      )
{
      TPMA_LOCALITY                 locality_attributes;
      BYTE                         *localityAsByte = (BYTE *)&locality_attributes;
      MemorySet(&locality_attributes, 0, sizeof(TPMA_LOCALITY));
      switch(locality)
      {
          case 0:
              locality_attributes.TPM_LOC_ZERO = SET;
              break;
          case 1:
              locality_attributes.TPM_LOC_ONE = SET;
              break;
          case 2:
              locality_attributes.TPM_LOC_TWO = SET;
              break;
          case 3:
              locality_attributes.TPM_LOC_THREE = SET;
              break;
          case 4:
              locality_attributes.TPM_LOC_FOUR = SET;
              break;
          default:
              pAssert(locality < 256 && locality > 31);
              *localityAsByte = locality;
              break;
      }
      return locality_attributes;
}
