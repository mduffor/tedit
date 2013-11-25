//
// GapBufferTest.cpp
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
#include "GapBufferTest.hpp"
#include <string.h>

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION (GapBufferTest);


//-----------------------------------------------------------------------------
void GapBufferTest::setUp()
{
}


//-----------------------------------------------------------------------------
void GapBufferTest::tearDown()
{
}

//-----------------------------------------------------------------------------
void GapBufferTest::testConstructor()
{
  GapBufferManager *  pManager = new GapBufferManager;
  
  pManager->CreateBuffer ("TestBuffer");
  CPPUNIT_ASSERT (pManager->GetBuffer ("Blah") == NULL);
  CPPUNIT_ASSERT (pManager->GetBuffer ("TestBuffer") != NULL);
  
  GapBuffer *  gapBuffer = pManager->GetBuffer ("TestBuffer");
  
  // Testing AllocBuffer, FillGap, GetBuffer
  gapBuffer->AllocBuffer (16);
  gapBuffer->FillGap ();
  const char * szRawBuffer = gapBuffer->GetBuffer();
  
  CPPUNIT_ASSERT (szRawBuffer[15] == '\0');
  CPPUNIT_ASSERT (strcmp (&szRawBuffer[0], "012345678901234") == 0);
  
  // Testing SetName, GetName, SetFileName, GetFileName
  CPPUNIT_ASSERT (strcmp (gapBuffer->GetName (), "TestBuffer") == 0);
  gapBuffer->SetName ("TestBufferRenamed");
  CPPUNIT_ASSERT (strcmp (gapBuffer->GetName (), "TestBufferRenamed") == 0);

  CPPUNIT_ASSERT (strcmp (gapBuffer->GetFileName (), "") == 0);
  gapBuffer->SetFileName ("/testBuffer.txt");
  CPPUNIT_ASSERT (strcmp (gapBuffer->GetFileName (), "/testBuffer.txt") == 0);
  
  // Testing InsertChar, InsertString, GetLineLength, DeleteChars
  CPPUNIT_ASSERT (gapBuffer->GetLineLength (1) == 0);
  
  gapBuffer->InsertChar ('a');
  CPPUNIT_ASSERT (szRawBuffer[0] == 'a');
  CPPUNIT_ASSERT (gapBuffer->GetLineLength (1) == 1);
  
  gapBuffer->InsertChar ('b');
  szRawBuffer = gapBuffer->GetBuffer();
  CPPUNIT_ASSERT (szRawBuffer[1] == 'b');
  CPPUNIT_ASSERT (gapBuffer->GetLineLength (1) == 2);

  gapBuffer->InsertString ("cde");
  szRawBuffer = gapBuffer->GetBuffer();
  CPPUNIT_ASSERT (szRawBuffer[2] == 'c');
  CPPUNIT_ASSERT (szRawBuffer[3] == 'd');
  CPPUNIT_ASSERT (szRawBuffer[4] == 'e');
  CPPUNIT_ASSERT (gapBuffer->GetLineLength (1) == 5);

  gapBuffer->DeleteChars (-1);
  gapBuffer->FillGap ();

  CPPUNIT_ASSERT (gapBuffer->GetLineLength (1) == 4);
  CPPUNIT_ASSERT (szRawBuffer[4] != 'e');
  CPPUNIT_ASSERT (szRawBuffer[3] == 'd');

  // Testing SetCursor, BeginEdit, DeleteChars
  gapBuffer->SetCursor (1, 2);
  CPPUNIT_ASSERT (gapBuffer->GetCursor () == Location (1, 2));
  gapBuffer->BeginEdit();
  gapBuffer->FillGap ();
  szRawBuffer = gapBuffer->GetBuffer();
  CPPUNIT_ASSERT (strncmp (&szRawBuffer[0], "ab23456789012cd", 15) == 0);
  CPPUNIT_ASSERT (gapBuffer->GetLineLength (1) == 4);

  gapBuffer->DeleteChars (1);
  CPPUNIT_ASSERT (gapBuffer->GetCursor () == Location (1, 2));
  gapBuffer->FillGap ();
  CPPUNIT_ASSERT (strncmp (&szRawBuffer[0], "ab234567890123d", 15) == 0);
  CPPUNIT_ASSERT (gapBuffer->GetLineLength (1) == 3);

  gapBuffer->ReplaceChar ('c');
  CPPUNIT_ASSERT (gapBuffer->GetCursor () == Location (1, 3));
  gapBuffer->FillGap ();
  CPPUNIT_ASSERT (strncmp (&szRawBuffer[0], "abc345678901234", 15) == 0);
  CPPUNIT_ASSERT (gapBuffer->GetLineLength (1) == 3);

  gapBuffer->ReplaceChar ('d');
  CPPUNIT_ASSERT (gapBuffer->GetCursor () == Location (1, 4));
  gapBuffer->FillGap ();
  CPPUNIT_ASSERT (strncmp (&szRawBuffer[0], "abcd45678901234", 15) == 0);
  CPPUNIT_ASSERT (gapBuffer->GetLineLength (1) == 4);

  gapBuffer->ReplaceString ("ef");
  CPPUNIT_ASSERT (gapBuffer->GetCursor () == Location (1, 6));
  gapBuffer->FillGap ();
  CPPUNIT_ASSERT (strncmp (&szRawBuffer[0], "abcdef678901234", 15) == 0);
  CPPUNIT_ASSERT (gapBuffer->GetLineLength (1) == 6);

  Location testLoc (1, 3);
  gapBuffer->SetCursor (testLoc);
  CPPUNIT_ASSERT (gapBuffer->GetCursor () == Location (1, 3));
  gapBuffer->ReplaceString ("DEF");
  gapBuffer->FillGap ();
  CPPUNIT_ASSERT (strncmp (&szRawBuffer[0], "abcDEF678901234", 15) == 0);
  CPPUNIT_ASSERT (gapBuffer->GetLineLength (1) == 6);
  CPPUNIT_ASSERT (gapBuffer->GetCursor () == Location (1, 6));

  // test Clear
  gapBuffer->Clear ();
  CPPUNIT_ASSERT (gapBuffer->GetLineLength (1) == 0);

  // test ReplaceChar and ReplaceString with linefeeds.
  gapBuffer->SetCursor (1,0);
  gapBuffer->ReplaceString ("abcd\nefgh\nijk");
  gapBuffer->FillGap ();
  szRawBuffer = gapBuffer->GetBuffer();
  CPPUNIT_ASSERT (strncmp (&szRawBuffer[0], "abcd\nefgh\nijk34", 15) == 0);
  CPPUNIT_ASSERT (gapBuffer->GetLineLength (1) == 4);
  CPPUNIT_ASSERT (gapBuffer->GetLineLength (2) == 4);
  CPPUNIT_ASSERT (gapBuffer->GetLineLength (3) == 3);

  gapBuffer->Clear ();
  gapBuffer->SetCursor (1,0);
  gapBuffer->ReplaceString ("abcd\nefgh");
  gapBuffer->FillGap ();
  gapBuffer->SetCursor (1,2);
  gapBuffer->ReplaceChar ('C');
  gapBuffer->FillGap ();
  CPPUNIT_ASSERT (strncmp (&szRawBuffer[0], "abC345678d\nefgh", 15) == 0);
  CPPUNIT_ASSERT (gapBuffer->GetLineLength (1) == 4);
  CPPUNIT_ASSERT (gapBuffer->GetCursor () == Location (1, 3));

  gapBuffer->ReplaceChar ('D');
  gapBuffer->ReplaceChar ('E');
  gapBuffer->FillGap ();
  CPPUNIT_ASSERT (strncmp (&szRawBuffer[0], "abCDE56789\nefgh", 15) == 0);
  CPPUNIT_ASSERT (gapBuffer->GetLineLength (1) == 5);
  CPPUNIT_ASSERT (gapBuffer->GetCursor () == Location (1, 5));
  
  gapBuffer->ReplaceString ("FGH");
  gapBuffer->FillGap ();
  CPPUNIT_ASSERT (strncmp (&szRawBuffer[0], "abCDEFGH89\nefgh", 15) == 0);
  CPPUNIT_ASSERT (gapBuffer->GetCursor () == Location (1, 8));
  
  // Test Resize
  gapBuffer->SetGrowSize (10);
  gapBuffer->ReplaceString ("IJKL");
  gapBuffer->FillGap ();
  szRawBuffer = gapBuffer->GetBuffer();
  CPPUNIT_ASSERT (strncmp (&szRawBuffer[0], "abCDEFGHIJKL23456789\nefgh", 25) == 0);
  CPPUNIT_ASSERT (gapBuffer->GetLineLength (1) == 12);
  CPPUNIT_ASSERT (gapBuffer->GetLineLength (2) == 4);
  CPPUNIT_ASSERT (gapBuffer->GetCursor () == Location (1, 12));

  // Test SetCursor past a linefeed
  gapBuffer->SetCursor (2,2);
  gapBuffer->ReplaceChar ('G');
  gapBuffer->FillGap ();
  CPPUNIT_ASSERT (strncmp (&szRawBuffer[0], "abCDEFGHIJKL\nefG67890123h", 25) == 0);
  CPPUNIT_ASSERT (gapBuffer->GetLineLength (1) == 12);
  CPPUNIT_ASSERT (gapBuffer->GetLineLength (2) == 4);
  CPPUNIT_ASSERT (gapBuffer->GetCursor () == Location (2, 3));

  // Test Inserting linefeed
  gapBuffer->Clear ();
  gapBuffer->SetCursor (1,0);
  gapBuffer->InsertString ("abcd\nefghijkl");
  gapBuffer->FillGap ();
  szRawBuffer = gapBuffer->GetBuffer();
  CPPUNIT_ASSERT (strncmp (&szRawBuffer[0], "abcd\nefghijkl345678901234", 25) == 0);
  gapBuffer->SetCursor (2,4);
  gapBuffer->InsertChar('\n');
  gapBuffer->FillGap ();
  CPPUNIT_ASSERT (strncmp (&szRawBuffer[0], "abcd\nefgh\n01234567890ijkl", 25) == 0);

  
  // Test SetCursor past buffer end
  gapBuffer->Clear ();
  gapBuffer->SetCursor (1,0);
  gapBuffer->InsertString ("abcd\nefgh");
  gapBuffer->SetCursor (4,1);
  gapBuffer->BeginEdit();
  gapBuffer->InsertChar ('A');
  gapBuffer->FillGap ();
  CPPUNIT_ASSERT (strncmp (&szRawBuffer[0], "abcd\nefgh\n\n A", 13) == 0);
  //gapBuffer->DebugPrintLineOffsets ();
  //DBG_INFO ("Line Lengths %d %d %d %d %d\n", gapBuffer->GetLineLength (1), gapBuffer->GetLineLength (2), gapBuffer->GetLineLength (3), gapBuffer->GetLineLength (4), gapBuffer->GetLineLength (5));
  CPPUNIT_ASSERT (gapBuffer->GetLineLength (1) == 4);
  CPPUNIT_ASSERT (gapBuffer->GetLineLength (2) == 4);
  CPPUNIT_ASSERT (gapBuffer->GetLineLength (3) == 0);
  CPPUNIT_ASSERT (gapBuffer->GetLineLength (4) == 2);
  CPPUNIT_ASSERT (gapBuffer->GetCursor () == Location (4, 2));

  // Test Save and Load
  gapBuffer->Clear ();
  gapBuffer->SetCursor (1,0);
  gapBuffer->InsertString ("abcd\nefgh");
  gapBuffer->SetCursor (2,1);
  gapBuffer->SetFileName ("test.txt");
  gapBuffer->Save();
  gapBuffer->Clear ();
  gapBuffer->Load();
  szRawBuffer = gapBuffer->GetBuffer();
  CPPUNIT_ASSERT (strncmp (&szRawBuffer[0], "abcd\nefgh", 9) == 0);

  // Test LoadInsert
  gapBuffer->Clear ();
  gapBuffer->SetCursor (1,0);
  gapBuffer->InsertString ("EFGH");
  gapBuffer->SetFileName ("test.txt");
  gapBuffer->Save();
  gapBuffer->Clear ();
  gapBuffer->SetCursor (1,0);
  gapBuffer->InsertString ("abcdijkl");
  gapBuffer->SetCursor (1,4);
  gapBuffer->LoadInsert();
  gapBuffer->FillGap ();
  szRawBuffer = gapBuffer->GetBuffer();
  CPPUNIT_ASSERT (strncmp (&szRawBuffer[0], "abcdEFGH8901234567ijkl", 22) == 0);
  
  // Test deletion and how line offsets update
  gapBuffer->Clear ();
  gapBuffer->SetCursor (1,0);
  gapBuffer->InsertString ("abcd\nefgh\nijkl");
  gapBuffer->SetCursor (1,4);
  CPPUNIT_ASSERT (gapBuffer->GetNumLines () == 3);
  gapBuffer->DeleteChars (1);
  CPPUNIT_ASSERT (gapBuffer->GetNumLines () == 2);
  
  
//  gapBuffer->DebugPrintLineOffsets ();
  
  //gapBuffer->FillGap ();
  //printf ("GapBuffer is : \n%s\n", gapBuffer->GetBuffer());
  
  pManager->DeleteBuffer ("TestBufferRenamed");
  gapBuffer = NULL;

  delete pManager;
  pManager = NULL;

  
  //CPPUNIT_FAIL( "not implemented" );
}

