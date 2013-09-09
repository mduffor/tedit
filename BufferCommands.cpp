//
// BufferCommands.cpp
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

#include <string.h>
#include "Types.hpp"
#include "Debug.hpp"
ASSERTFILE (__FILE__)

#include "Command.hpp"
#include "GapBuffer.hpp"
#include "GapBufferManager.hpp"
#include "BufferCommands.hpp"

static GapBufferManager * pGapBufferManager = NULL;

//-----------------------------------------------------------------------------
VOID InitBufferCommands (GapBufferManager *  pBufferManagerIn,
                         CommandManager &    cmdManagerIn)
  {
  pGapBufferManager = pBufferManagerIn;
  
  cmdManagerIn.AddCommand ("CursorUp", CmdCursorUp);
  cmdManagerIn.AddCommand ("CursorDown", CmdCursorDown);
  cmdManagerIn.AddCommand ("CursorLeft", CmdCursorLeft);
  cmdManagerIn.AddCommand ("CursorRight", CmdCursorRight);
  
  cmdManagerIn.AddCommand ("CursorNextWord", CmdCursorNextWord);
  cmdManagerIn.AddCommand ("CursorPrevWord", CmdCursorPrevWord);
  cmdManagerIn.AddCommand ("CursorStartLine", CmdCursorStartLine);
  cmdManagerIn.AddCommand ("CursorEndLine", CmdCursorEndLine);
  cmdManagerIn.AddCommand ("CursorStartDoc", CmdCursorStartDoc);
  cmdManagerIn.AddCommand ("CursorEndDoc", CmdCursorEndDoc);
  };
  
  
//-----------------------------------------------------------------------------
VOID CmdCursorUp (RStrArray *  arrayParams)
  {
  ASSERT (pGapBufferManager != NULL);
  
  GapBuffer *  pBuffer = pGapBufferManager->GetCurrent ();
  Location  locCursor = pBuffer->GetCursor ();
  if (locCursor.iLine > 1) {
    locCursor.iLine -= 1;
    pBuffer->SetCursor (locCursor);
    };
  // not undoable  
  };

//-----------------------------------------------------------------------------
VOID CmdCursorDown (RStrArray *  arrayParams)
  {
  ASSERT (pGapBufferManager != NULL);
  
  GapBuffer *  pBuffer = pGapBufferManager->GetCurrent ();
  Location  locCursor = pBuffer->GetCursor ();
  locCursor.iLine += 1;
  pBuffer->SetCursor (locCursor);
  // not undoable  
  };

//-----------------------------------------------------------------------------
VOID CmdCursorLeft (RStrArray *  arrayParams)
  {
  ASSERT (pGapBufferManager != NULL);
  
  GapBuffer *  pBuffer = pGapBufferManager->GetCurrent ();
  Location  locCursor = pBuffer->GetCursor ();
  if (locCursor.iCol > 0) {
    locCursor.iCol -= 1;
    }
  else 
    {
    // wrap to previous line
    if (locCursor.iLine > 1)
      {
      locCursor.iLine -= 1;
      locCursor.iCol = pBuffer->GetLineLength (locCursor.iLine);
      }
    };
  pBuffer->SetCursor (locCursor);
  // not undoable  
  };

//-----------------------------------------------------------------------------
VOID CmdCursorRight (RStrArray *  arrayParams)
  {
  ASSERT (pGapBufferManager != NULL);
  
  GapBuffer *  pBuffer = pGapBufferManager->GetCurrent ();
  Location  locCursor = pBuffer->GetCursor ();
  locCursor.iCol += 1;
  pBuffer->SetCursor (locCursor);
  // not undoable  
  };

//-----------------------------------------------------------------------------
VOID CmdCursorStartDoc (RStrArray *  arrayParams)
  {
  ASSERT (pGapBufferManager != NULL);
  
  GapBuffer *  pBuffer = pGapBufferManager->GetCurrent ();
  pBuffer->SetCursor (1, 0);
  // not undoable  
  };

//-----------------------------------------------------------------------------
VOID CmdCursorEndDoc (RStrArray *  arrayParams)
  {
  ASSERT (pGapBufferManager != NULL);
  
  GapBuffer *  pBuffer = pGapBufferManager->GetCurrent ();
  INT  iLastLine = pBuffer->GetNumLines ();
  pBuffer->SetCursor (iLastLine, pBuffer->GetLineLength (iLastLine));
  // not undoable  
  };

//-----------------------------------------------------------------------------
VOID CmdCursorStartLine (RStrArray *  arrayParams)
  {
  ASSERT (pGapBufferManager != NULL);
  
  GapBuffer *  pBuffer = pGapBufferManager->GetCurrent ();
  Location  locCursor = pBuffer->GetCursor ();
  locCursor.iCol = 0;
  pBuffer->SetCursor (locCursor);
  // not undoable  
  };

//-----------------------------------------------------------------------------
VOID CmdCursorEndLine (RStrArray *  arrayParams)
  {
  ASSERT (pGapBufferManager != NULL);
  
  GapBuffer *  pBuffer = pGapBufferManager->GetCurrent ();
  Location  locCursor = pBuffer->GetCursor ();
  locCursor.iCol = pBuffer->GetLineLength (locCursor.iLine);
  pBuffer->SetCursor (locCursor);
  // not undoable  
  };

//-----------------------------------------------------------------------------
VOID CmdCursorNextWord (RStrArray *  arrayParams)
  {
  ASSERT (pGapBufferManager != NULL);
  
  GapBuffer *  pBuffer = pGapBufferManager->GetCurrent ();
  Location  locCursor = pBuffer->GetCursor ();

  
  locCursor.iCol += 1;
  pBuffer->SetCursor (locCursor);
  // not undoable  
  };

//-----------------------------------------------------------------------------
VOID CmdCursorPrevWord (RStrArray *  arrayParams)
  {
  ASSERT (pGapBufferManager != NULL);
  
  GapBuffer *  pBuffer = pGapBufferManager->GetCurrent ();
  Location  locCursor = pBuffer->GetCursor ();

  
  locCursor.iCol += 1;
  pBuffer->SetCursor (locCursor);
  // not undoable  
  };

  
  