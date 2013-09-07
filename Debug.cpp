/* -----------------------------------------------------------------
                             Debug.cpp

     This class is for displaying error, warning, and debugging
       messages to the user and the error log.

                            Michael T. Duffy
   ----------------------------------------------------------------- */

// Debug.cpp
// Copyright 2002, 2008 Michael T. Duffy.  All Rights Reserved
// contact:  mduffor@gmail.com

// This file is part of TEdit.
//
// TEdit is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 3 as
// published by the Free Software Foundation.
//
// TEdit is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with TEdit.  If not, see <http://www.gnu.org/licenses/>.


// ***********************************************************************
//  Compile Directives
// ***********************************************************************

// #pragma pack (1);

// ***********************************************************************
//  Header Files
// ***********************************************************************

#include <stdio.h>
#include <stdlib.h>
#include "RStr.hpp"

#include "Types.hpp"

#include "Debug.hpp"
ASSERTFILE (__FILE__)

#ifdef ANDROID_NDK
  #include <jni.h>    
  #include <android/log.h>
#endif // ANDROID_NDK

// ***********************************************************************
//  Module Variables
// ***********************************************************************


// Define the singleton

PDebugMessages  pdbgMessages = NULL;

RStr    strDbgBuffer;


// ***********************************************************************
//  DebugMessages
// ***********************************************************************


//------------------------------------------------------------------------
DebugMessages::DebugMessages  ()
  {
  };


//------------------------------------------------------------------------
DebugMessages::~DebugMessages ()
  {
  };


//------------------------------------------------------------------------
void  DebugMessages::Assert  (UINT32         uStatusIn,
                               const RStr &   strFileIn,
                               UINT32         uLineIn)
  {

  if (uStatusIn)
    {
    // the status checks out.  Do nothing.
    }
  else
    {
    // failed status check.  Display a message.

    RStr strMsgOut;

    strMsgOut.Format ("ASSERT ERROR in file %s at line %d.", strFileIn.AsChar (), uLineIn);

    RawWriteLog (strMsgOut.AsChar ());
    RawUserDisp ("Assert", strMsgOut.AsChar ());
    };
  };


//------------------------------------------------------------------------
void  DebugMessages::RunMark  (const char *  pszTextIn,
                                const RStr &  strFileIn,
                                UINT32        uLineIn)
  {
  RStr strMsgOut;

  strMsgOut.Format ("RUN MARK in file %s at line %d: %s", strFileIn.AsChar (), uLineIn, pszTextIn);

  RawWriteLog (strMsgOut.AsChar ());
  };

  
//------------------------------------------------------------------------
void  DebugMessages::Info  (const char *  pszTextIn,
                             const RStr &  strFileIn,
                             UINT32        uLineIn)
  {
  RStr strMsgOut;

  // For info messages, we will only display the file name, not the entire path.
  
  strMsgOut.Format ("INFO (%s:%d): %s", 
                    strFileIn.AsChar (strFileIn.ReverseFindChar ('/') + 1), 
                    uLineIn, 
                    pszTextIn);

  RawWriteLog (strMsgOut.AsChar ());
  };

  
//------------------------------------------------------------------------
void  DebugMessages::Warning  (const char *  pszTextIn,
                                const RStr &  strFileIn,
                                UINT32        uLineIn)
  {
  RStr strMsgOut;

  strMsgOut.Format ("WARNING (%s:%d): %s", strFileIn.AsChar (strFileIn.ReverseFindChar ('/') + 1), uLineIn, pszTextIn);

  RawWriteLog (strMsgOut.AsChar ());
  };


//------------------------------------------------------------------------
void  DebugMessages::Error  (const char *  pszTextIn,
                              const RStr &  strFileIn,
                              UINT32        uLineIn)
  {
  RStr strMsgOut;

  strMsgOut.Format ("***** ERROR!!! (%s:%d): %s *****", strFileIn.AsChar (strFileIn.ReverseFindChar ('/') + 1), uLineIn, pszTextIn);

  RawWriteLog (strMsgOut.AsChar ());
  RawUserDisp ("Error!!!", strMsgOut.AsChar ());
  };


//------------------------------------------------------------------------
void  DebugMessages::RawError  (const char *  pszTextIn)
  {
  RStr strMsgOut (pszTextIn);

  RawWriteLog (strMsgOut.AsChar ());
  RawUserDisp ("Error!!!", strMsgOut.AsChar ());
  };


//------------------------------------------------------------------------
void  DebugMessages::RunMark  (const RStr &  strTextIn,
                                const RStr &  strFileIn,
                                UINT32        uLineIn)
  {
  RunMark (strTextIn.AsChar (), strFileIn, uLineIn);
  };


//------------------------------------------------------------------------
void  DebugMessages::Warning  (const RStr &  strTextIn,
                                const RStr &  strFileIn,
                                UINT32        uLineIn)
  {
  Warning (strTextIn.AsChar (), strFileIn, uLineIn);
  };


//------------------------------------------------------------------------
void  DebugMessages::Error  (const RStr &  strTextIn,
                              const RStr &  strFileIn,
                              UINT32        uLineIn)
  {
  Error (strTextIn.AsChar (), strFileIn, uLineIn);
  };


//------------------------------------------------------------------------
void  DebugMessages::RawError  (const RStr &  strTextIn)
  {
  RawError (strTextIn.AsChar ());
  };


//------------------------------------------------------------------------
void  DebugMessages::RawUserDisp  (const char *  pszDialogTitleIn,
                                    const char *  pszTextIn)
  {
  // There is no default behavior for displaying user messages.  Generally anything
  //  that will display a user message will also write out a log message.

  printf ("%s\n", pszTextIn);
  };


//------------------------------------------------------------------------
void  DebugMessages::RawWriteLog  (const char *  pszTextIn)
  {
  FILE *             fp;


  // The default behavior for RawWriteLog is to display a message to a
  //   file called "debug.err".

  // we open and close the file each time in order to assure that the
  //  message is written, even if we crash.

  #ifdef ANDROID_NDK
    __android_log_print(ANDROID_LOG_INFO, "RavenDebug", "%s", pszTextIn);
  #else
    printf ("%s\n", pszTextIn);
    fp = fopen ("debug.err", "at");
    if (fp != NULL)
      {
      fprintf (fp, "%s\n", pszTextIn);
      fclose (fp);
      };
  #endif
  };



// ***********************************************************************
//  DebugMessagesFactory
// ***********************************************************************



//------------------------------------------------------------------------
DebugMessagesFactory::DebugMessagesFactory  ()
  {
  };


//------------------------------------------------------------------------
DebugMessagesFactory::~DebugMessagesFactory ()
  {
  };


//------------------------------------------------------------------------
PDebugMessages  DebugMessagesFactory::Initialize  (VOID)
  {
  #ifdef ANDROID_NDK
  #else
    remove ("debug.err");
  #endif
  
  SetSingleton (new DebugMessages);

  return (pdbgMessages);
  };


//------------------------------------------------------------------------
void  DebugMessagesFactory::Uninitialize  (VOID)
  {
  delete pdbgMessages;
  SetSingleton (NULL);
  };


//------------------------------------------------------------------------
void  DebugMessagesFactory::SetSingleton  (PDebugMessages  pdbgMessagesIn)
  {
  pdbgMessages = pdbgMessagesIn;
  };
