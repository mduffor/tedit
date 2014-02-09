//
// Main.cpp
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

#include <stdio.h>
#include "GapBuffer.hpp"
#include "GapBufferManager.hpp"
#include "NCursesShell.hpp"
#include "FormatInfo.hpp"
#include "EditorSettings.hpp"
#include "Command.hpp"
#include "BufferCommands.hpp"
#include "EntryFieldHandler.hpp"
#include "FilePath.hpp"


#include "Debug.hpp"
ASSERTFILE (__FILE__)

//-----------------------------------------------------------------------------
int main (int argc, char *argv[])
  {
  printf ("Hello World\n");
  
  GapBufferManager *  pGapManager = new GapBufferManager;
  FormatInfo *        pFormat = new FormatInfo;
  EditorSettings *    pSettings = new EditorSettings;
  CommandManager      cmdManager;  
  EntryFieldHandler   entryFieldHandler;
  
  InitBufferCommands (pGapManager, cmdManager, pFormat, pSettings, &entryFieldHandler);
  
  // TODO: Expand this to a full command line parser
  // open files from the command line
  FilePath  filePath;
  INT  iFilesOpened = 0;
  for (INT  iIndex = 1; iIndex < argc; ++iIndex)
    {
    if (FilePath::FileExists (argv[iIndex]))
      {
      pGapManager->OpenFile (argv[iIndex]);
      ++iFilesOpened;
      }
    }

  if (iFilesOpened == 0)
    {
    pGapManager->OpenFile ("./SyntaxTest.txt");
    //GapBuffer *  gapBuffer = pGapManager->OpenFile ("./SyntaxTest.txt");
    GapBuffer *  gapBuffer = pGapManager->OpenFile ("./test/helloWorld2.txt");

  
    gapBuffer->SetCursor(10, 3);
    };
  
  //gapBuffer->InsertString ("The quick brown fox.\n");
  //const char * szRawBuffer = gapBuffer->GetBuffer();
  //printf ("%s\n", szRawBuffer);
  //printf ("Line Length 0: %d\n", gapBuffer->GetLineLength(0));
  //printf ("Line Length 1:  %d\n", gapBuffer->GetLineLength(1));
  
  //gapBuffer->DebugPrintLineOffsets ();
  //gapBuffer->BeginEdit();
  //gapBuffer->DebugPrintLineOffsets ();
  
  
  
  NCursesShell *  pShell = new NCursesShell ();
  pShell->Update (pGapManager, cmdManager, *pSettings, entryFieldHandler);
  delete (pShell);
  
  return (0);
  }
  
  