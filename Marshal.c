// This file was extracted from the TCG Published
// Trusted Platform Module Library
// Part 4: Supporting Routines
// Family "2.0"
// Level 00 Revision 01.16
// October 30, 2014

typedef struct {
   UINT16              size;
   BYTE                buffer[1];
} TPM2B;
typedef struct {
   UINT16              size;
   BYTE                buffer[1024];
} EVENT_2B;
typedef union {
   EVENT_2B            t;      // The type-specific union member
   TPM2B               b;      // The generic union member
} TPM2B_EVENT;
