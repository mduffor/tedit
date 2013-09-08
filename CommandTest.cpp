//
// CommandTest.cpp
// Copyright 2013  Michael T. Duffy.  
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
//


#include "Types.hpp"
#include "Debug.hpp"
ASSERTFILE (__FILE__)

#include "GapBuffer.hpp"
#include "GapBufferManager.hpp"
#include "Command.hpp"
#include "BufferCommands.hpp"
#include "CommandTest.hpp"
#include <string.h>

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION (CommandTest);


//-----------------------------------------------------------------------------
void CommandTest::setUp ()
{
}


//-----------------------------------------------------------------------------
void CommandTest::tearDown ()
{
}


//-----------------------------------------------------------------------------
INT  iGlobalInt = 0;
VOID CmdSetGlobalInt (RStrArray *  arrayParams)
  {
  iGlobalInt = 1;
  }

//-----------------------------------------------------------------------------
void CommandTest::testCommandManager ()
  {
  CommandManager     cmdManager;  

  CPPUNIT_ASSERT (cmdManager.FindCommand ("SetGlobalInt") == NULL);
  cmdManager.AddCommand ("SetGlobalInt", CmdSetGlobalInt);
  CPPUNIT_ASSERT (cmdManager.FindCommand ("SetGlobalInt") != NULL);

  cmdManager.AddCommand ("SetGlobalInt", CmdSetGlobalInt);
  cmdManager.AddCommand ("SetGlobalInt2", CmdSetGlobalInt);
  cmdManager.AddCommand ("SetGlobalInt3", CmdSetGlobalInt);
  cmdManager.AddCommand ("SetGlobalInt4", CmdSetGlobalInt);
  cmdManager.AddCommand ("SetGlobalInt5", CmdSetGlobalInt);
  cmdManager.AddCommand ("SetGlobalInt6", CmdSetGlobalInt);
  
  CPPUNIT_ASSERT (cmdManager.FindCommand ("SetGlobalInt") != NULL);
  CPPUNIT_ASSERT (cmdManager.FindCommand ("SetGlobalInt3") != NULL);
  CPPUNIT_ASSERT (cmdManager.FindCommand ("SetGlobalInt7") == NULL);
  
  CPPUNIT_ASSERT (iGlobalInt == 0);
  cmdManager.ExecuteCommand ("SetGlobalInt", NULL);
  CPPUNIT_ASSERT (iGlobalInt == 1);
  }

/*
    VOID       ParseCommand    (RStr &       strIn,
                                RStr &       strNameOut,
                                RStrArray *  arrayParamsOut);
                             
    VOID       FreeCommands    (VOID);                    
    
    VOID       AddCommand      (const char *  szNameIn,
                                VOID (*fnIn)(RStrArray *));
                        
    Command *  FindCommand     (const char *  szNameIn);
    
    VOID ExecuteCommand        (const char *  szNameIn, 
                                RStrArray *   arrayParams);
*/

//-----------------------------------------------------------------------------
void CommandTest::testCommand ()
{

  GapBufferManager *  pManager = new GapBufferManager;
  pManager->CreateBuffer ("TestBuffer");
  GapBuffer *  gapBuffer = pManager->GetBuffer ("TestBuffer");
  gapBuffer->AllocBuffer (40);
//  gapBuffer->FillGap ();
//  const char * szRawBuffer = gapBuffer->GetBuffer();
  
//  CPPUNIT_ASSERT (strcmp (&szRawBuffer[0], "012345678901234") == 0);

  gapBuffer->SetCursor (1, 0);
  gapBuffer->InsertString ("abcd efgh ijkl\nmn op qr st\nu v w x\nyz\n");

  // test buffer commands
  CommandManager     cmdManager;  
  InitBufferCommands (pManager, cmdManager);
  
  
  gapBuffer->SetCursor (3, 1);
  CPPUNIT_ASSERT (gapBuffer->GetCursor () == Location (3, 1));
  cmdManager.ExecuteCommand ("CursorUp", NULL);
  CPPUNIT_ASSERT (gapBuffer->GetCursor () == Location (2, 1));
  cmdManager.ExecuteCommand ("CursorUp", NULL);
  CPPUNIT_ASSERT (gapBuffer->GetCursor () == Location (1, 1));
  cmdManager.ExecuteCommand ("CursorUp", NULL);
  CPPUNIT_ASSERT (gapBuffer->GetCursor () == Location (1, 1));
  
  //CPPUNIT_ASSERT (strncmp (gapBuffer->GetBuffer(), "abcd efgh ijkl\nmn op qr st\nu v w x\nyz\n8", 38) == 0);
  cmdManager.ExecuteCommand ("CursorDown", NULL);
  CPPUNIT_ASSERT (gapBuffer->GetCursor () == Location (2, 1));
  cmdManager.ExecuteCommand ("CursorDown", NULL);
  CPPUNIT_ASSERT (gapBuffer->GetCursor () == Location (3, 1));
  
  cmdManager.ExecuteCommand ("CursorLeft", NULL);
  CPPUNIT_ASSERT (gapBuffer->GetCursor () == Location (3, 0));
  cmdManager.ExecuteCommand ("CursorLeft", NULL);
  CPPUNIT_ASSERT (gapBuffer->GetCursor () == Location (2, 11));
  
  cmdManager.ExecuteCommand ("CursorRight", NULL);
  CPPUNIT_ASSERT (gapBuffer->GetCursor () == Location (2, 12));
  cmdManager.ExecuteCommand ("CursorRight", NULL);
  CPPUNIT_ASSERT (gapBuffer->GetCursor () == Location (2, 13));
  
  //gapBuffer->FillGap ();
  //printf ("GapBuffer is : \n%s\n", gapBuffer->GetBuffer());
  //printf ("Cursor is %d %d\n", gapBuffer->GetCursor ().iLine, gapBuffer->GetCursor ().iCol);
  
  pManager->DeleteBuffer ("TestBuffer");
  gapBuffer = NULL;

  delete pManager;
  pManager = NULL;
  //CPPUNIT_FAIL( "not implemented" );
}

