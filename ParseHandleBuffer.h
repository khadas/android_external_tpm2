// This file was extracted from the TCG Published
// Trusted Platform Module Library
// Part 4: Supporting Routines
// Family "2.0"
// Level 00 Revision 01.16
// October 30, 2014

#ifndef         _HANDLEPROCESS_FP_H_
#define         _HANDLEPROCESS_FP_H_
TPM_RC
ParseHandleBuffer(
     TPM_CC         commandCode,                  //   IN: Command being processed
     BYTE           **handleBufferStart,          //   IN/OUT: command buffer where handles
                                                  //     are located. Updated as handles
                                                  //     are unmarshaled
     INT32          *bufferRemainingSize,         //   IN/OUT: indicates the amount of data
                                                  //     left in the command buffer.
                                                  //     Updated as handles are unmarshaled
   TPM_HANDLE handles[],                          //   OUT: Array that receives the handles
   UINT32     *handleCount                        //   OUT: Receives the count of handles
   );
#endif // _HANDLEPROCESS_FP_H_
