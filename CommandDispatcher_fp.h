// This file was extracted from the TCG Published
// Trusted Platform Module Library
// Part 4: Supporting Routines
// Family "2.0"
// Level 00 Revision 01.16
// October 30, 2014

#ifndef        _COMMANDDISPATCHER_FP_H_
#define        _COMMANDDISPATCHER_FP_H_
TPM_RC
CommandDispatcher(
   TPMI_ST_COMMAND_TAG      tag,   //             IN: Input command tag
   TPM_CC      commandCode,        //             IN: Command code
   INT32       *parmBufferSize,    //             IN: size of parameter buffer
   BYTE        *parmBufferStart,   //             IN: pointer to start of parameter buffer
   TPM_HANDLE handles[],           //             IN: handle array
   UINT32      *responseHandleSize,//             OUT: size of handle buffer in response
   UINT32      *respParmSize       //             OUT: size of parameter buffer in response
   );
#endif // _COMMANDDISPATCHER_FP_H_
