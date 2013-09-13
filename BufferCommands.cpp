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
#include "EditorSettings.hpp"

static GapBufferManager * pGapBufferManager = NULL;
static FormatInfo *       pFormatInfo = NULL;
static EditorSettings *   pSettings = NULL;

//-----------------------------------------------------------------------------
VOID InitBufferCommands (GapBufferManager *  pBufferManagerIn,
                         CommandManager &    cmdManagerIn,
                         FormatInfo *        pFormatIn,
                         EditorSettings *    pSettingsIn)
  {
  ASSERT (pBufferManagerIn != NULL);
  ASSERT (pFormatIn != NULL);
  ASSERT (pSettingsIn != NULL);
  
  pGapBufferManager = pBufferManagerIn;
  pFormatInfo = pFormatIn;
  pSettings = pSettingsIn;

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
  
  cmdManagerIn.AddCommand ("SelectUp", CmdCursorUp);
  cmdManagerIn.AddCommand ("SelectDown", CmdCursorDown);
  cmdManagerIn.AddCommand ("SelectLeft", CmdCursorLeft);
  cmdManagerIn.AddCommand ("SelectRight", CmdCursorRight);
  
  cmdManagerIn.AddCommand ("SelectNextWord", CmdCursorNextWord);
  cmdManagerIn.AddCommand ("SelectPrevWord", CmdCursorPrevWord);
  cmdManagerIn.AddCommand ("SelectStartLine", CmdCursorStartLine);
  cmdManagerIn.AddCommand ("SelectEndLine", CmdCursorEndLine);
  cmdManagerIn.AddCommand ("SelectStartDoc", CmdCursorStartDoc);
  cmdManagerIn.AddCommand ("SelectEndDoc", CmdCursorEndDoc);
  
  cmdManagerIn.AddCommand ("SelectionCut", CmdSelectionCut);
  };

//-----------------------------------------------------------------------------
VOID StartSelection (VOID)
  {
  GapBuffer *  pBuffer = pGapBufferManager->GetCurrent ();
  if (!pBuffer->IsSelectionValid ())
    {
    pBuffer->SetSelection (pBuffer->GetCursor ());
    };
  };
  
//-----------------------------------------------------------------------------
VOID DismissSelection (VOID)
  {
  GapBuffer *  pBuffer = pGapBufferManager->GetCurrent ();
  pBuffer->ClearSelection ();
  };
  
//-----------------------------------------------------------------------------
VOID CmdCursorUp (RStrArray *  arrayParams)
  {
  DismissSelection ();
  CursorUp ();
  };

//-----------------------------------------------------------------------------
VOID CmdCursorDown (RStrArray *  arrayParams)
  {
  DismissSelection ();
  CursorDown ();
  };

//-----------------------------------------------------------------------------
VOID CmdCursorLeft (RStrArray *  arrayParams)
  {
  DismissSelection ();
  CursorLeft ();
  };

//-----------------------------------------------------------------------------
VOID CmdCursorRight (RStrArray *  arrayParams)
  {
  DismissSelection ();
  CursorRight ();
  };

//-----------------------------------------------------------------------------
VOID CmdCursorStartDoc (RStrArray *  arrayParams)
  {
  DismissSelection ();
  CursorStartDoc ();
  };

//-----------------------------------------------------------------------------
VOID CmdCursorEndDoc (RStrArray *  arrayParams)
  {
  DismissSelection ();
  CursorEndDoc ();
  };

//-----------------------------------------------------------------------------
VOID CmdCursorStartLine (RStrArray *  arrayParams)
  {
  DismissSelection ();
  CursorStartLine ();
  };

//-----------------------------------------------------------------------------
VOID CmdCursorEndLine (RStrArray *  arrayParams)
  {
  DismissSelection ();
  CursorEndLine ();
  };

//-----------------------------------------------------------------------------
VOID CmdCursorNextWord (RStrArray *  arrayParams)
  {
  DismissSelection ();
  CursorNextWord ();
  };

//-----------------------------------------------------------------------------
VOID CmdCursorPrevWord (RStrArray *  arrayParams)
  {
  DismissSelection ();
  CursorPrevWord ();
  };
  
//-----------------------------------------------------------------------------
VOID CmdSelectUp (RStrArray *  arrayParams)
  {
  StartSelection ();
  CursorUp ();
  };

//-----------------------------------------------------------------------------
VOID CmdSelectDown (RStrArray *  arrayParams)
  {
  StartSelection ();
  CursorDown ();
  };

//-----------------------------------------------------------------------------
VOID CmdSelectLeft (RStrArray *  arrayParams)
  {
  StartSelection ();
  CursorLeft ();
  };

//-----------------------------------------------------------------------------
VOID CmdSelectRight (RStrArray *  arrayParams)
  {
  StartSelection ();
  CursorRight ();
  };

//-----------------------------------------------------------------------------
VOID CmdSelectStartDoc (RStrArray *  arrayParams)
  {
  StartSelection ();
  CursorStartDoc ();
  };

//-----------------------------------------------------------------------------
VOID CmdSelectEndDoc (RStrArray *  arrayParams)
  {
  StartSelection ();
  CursorEndDoc ();
  };

//-----------------------------------------------------------------------------
VOID CmdSelectStartLine (RStrArray *  arrayParams)
  {
  StartSelection ();
  CursorStartLine ();
  };

//-----------------------------------------------------------------------------
VOID CmdSelectEndLine (RStrArray *  arrayParams)
  {
  StartSelection ();
  CursorEndLine ();
  };

//-----------------------------------------------------------------------------
VOID CmdSelectNextWord (RStrArray *  arrayParams)
  {
  StartSelection ();
  CursorNextWord ();
  };

//-----------------------------------------------------------------------------
VOID CmdSelectPrevWord (RStrArray *  arrayParams)
  {
  StartSelection ();
  CursorPrevWord ();
  };
  
//-----------------------------------------------------------------------------
VOID CursorUp (VOID)
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
VOID CursorDown (VOID)
  {
  ASSERT (pGapBufferManager != NULL);
  
  GapBuffer *  pBuffer = pGapBufferManager->GetCurrent ();
  Location  locCursor = pBuffer->GetCursor ();
  locCursor.iLine += 1;
  pBuffer->SetCursor (locCursor);
  // not undoable  
  };

//-----------------------------------------------------------------------------
VOID CursorLeft (VOID)
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
VOID CursorRight (VOID)
  {
  ASSERT (pGapBufferManager != NULL);
  
  GapBuffer *  pBuffer = pGapBufferManager->GetCurrent ();
  Location  locCursor = pBuffer->GetCursor ();
  locCursor.iCol += 1;
  pBuffer->SetCursor (locCursor);
  // not undoable  
  };

//-----------------------------------------------------------------------------
VOID CursorStartDoc ()
  {
  ASSERT (pGapBufferManager != NULL);
  
  GapBuffer *  pBuffer = pGapBufferManager->GetCurrent ();
  pBuffer->SetCursor (1, 0);
  // not undoable  
  };

//-----------------------------------------------------------------------------
VOID CursorEndDoc (VOID)
  {
  ASSERT (pGapBufferManager != NULL);
  
  GapBuffer *  pBuffer = pGapBufferManager->GetCurrent ();
  INT  iLastLine = pBuffer->GetNumLines ();
  pBuffer->SetCursor (iLastLine, pBuffer->GetLineLength (iLastLine));
  // not undoable  
  };

//-----------------------------------------------------------------------------
VOID CursorStartLine (VOID)
  {
  ASSERT (pGapBufferManager != NULL);
  
  GapBuffer *  pBuffer = pGapBufferManager->GetCurrent ();
  Location  locCursor = pBuffer->GetCursor ();
  locCursor.iCol = 0;
  pBuffer->SetCursor (locCursor);
  // not undoable  
  };

//-----------------------------------------------------------------------------
VOID CursorEndLine (VOID)
  {
  ASSERT (pGapBufferManager != NULL);
  
  GapBuffer *  pBuffer = pGapBufferManager->GetCurrent ();
  Location  locCursor = pBuffer->GetCursor ();
  locCursor.iCol = pBuffer->GetLineLength (locCursor.iLine);
  pBuffer->SetCursor (locCursor);
  // not undoable  
  };

//-----------------------------------------------------------------------------
VOID CursorNextWord (VOID)
  {
  ASSERT (pGapBufferManager != NULL);
  
  GapBuffer *  pBuffer = pGapBufferManager->GetCurrent ();
  Location  locCursor = pBuffer->GetCursor ();

  // if beyond buffer, do nothing
  if (locCursor.iLine > pBuffer->GetNumLines())
    {
    return;
    }
  
  // if at line end, go to line beginning
  INT  iCurrChar = pBuffer->GetCharAtLocation (locCursor);
  if (pFormatInfo->IsEOL (iCurrChar) || (iCurrChar == -1))
    {
    locCursor.iLine += 1;
    locCursor.iCol = 0;
    pBuffer->SetCursor (locCursor);
    return;
    }
  // move forward past any word characters you are on
  while (pFormatInfo->IsWordChar (pBuffer->GetCharAtLocation (locCursor))) 
    {
    locCursor.iCol += 1;
    };
  // move past any non-word characters, such as whitespace and punctuation  
  iCurrChar = pBuffer->GetCharAtLocation (locCursor);
  while ((!pFormatInfo->IsWordChar (iCurrChar)) && 
            (!pFormatInfo->IsEOL (iCurrChar)) && 
            (iCurrChar != -1))
    {
    locCursor.iCol += 1;
    iCurrChar = pBuffer->GetCharAtLocation (locCursor);
    }

  pBuffer->SetCursor (locCursor);
  // not undoable  
  };

//-----------------------------------------------------------------------------
VOID CursorPrevWord (VOID)
  {
  ASSERT (pGapBufferManager != NULL);
  
  GapBuffer *  pBuffer = pGapBufferManager->GetCurrent ();
  Location  locCursor = pBuffer->GetCursor ();

  // if beyond buffer, go to end
  if (locCursor.iLine > pBuffer->GetNumLines())
    {
    INT  iLastLine = pBuffer->GetNumLines ();
    pBuffer->SetCursor (iLastLine, pBuffer->GetLineLength (iLastLine));
    return;
    }
  
  // if at line beginning, go to prev line end
  if ((locCursor.iCol == 0) && (locCursor.iLine > 1))
    {
    --locCursor.iLine;
    locCursor.iCol = pBuffer->GetLineLength (locCursor.iLine);
    pBuffer->SetCursor (locCursor);
    return;
    }
    
  // move past any non-word characters, such as whitespace and punctuation  
  while ((locCursor.iCol != 0) &&
         (!pFormatInfo->IsWordChar (pBuffer->GetCharAtLocation (locCursor.iLine, locCursor.iCol - 1))))
    {
    --locCursor.iCol;
    }

  // move forward past any word characters you are on
  while ((locCursor.iCol != 0) &&
         (pFormatInfo->IsWordChar (pBuffer->GetCharAtLocation (locCursor.iLine, locCursor.iCol - 1))))
    {
    --locCursor.iCol;
    };
  pBuffer->SetCursor (locCursor);
  // not undoable  
  };

//-----------------------------------------------------------------------------
VOID SelectionCopy (VOID)
  {
  ASSERT (pGapBufferManager != NULL);
  GapBuffer *  pBuffer = pGapBufferManager->GetCurrent ();

  Location locCursor = pBuffer->GetCursor ();
  Location locSelect = pBuffer->GetSelection ();
  
  Location locBegin = locCursor;
  Location locEnd   = locSelect;
  
  if (locBegin > locEnd)
    {
    locBegin = locSelect;
    locEnd   = locCursor;
    }

  pBuffer->ClampLocationToValidChar (locBegin);
  pBuffer->ClampLocationToValidChar (locEnd);
    

  // copy selected text to clip
  pBuffer->SetCursor (locBegin);
  INT  iCharsToCopy = pBuffer->GetCharsBetween (locBegin, locEnd);
  
  RStr &  strClip = pSettings->GetClip ();
  strClip.GrowAbsolute (iCharsToCopy + 1);
  strClip.SetAt (iCharsToCopy, '\0');
  
  pBuffer->GetString (strClip.GetBufferPtr (), iCharsToCopy);
  //printf ("Copied %d chars (%d, %d) to (%d, %d):\n", iCharsToCopy, locBegin.iLine, locBegin.iCol, locEnd.iLine, locEnd.iCol);
  //printf (strClip.AsChar ());
  //printf ("\n");
  
  // undoable  
  
  };

//-----------------------------------------------------------------------------
VOID SelectionDelete (VOID)
  {
  ASSERT (pGapBufferManager != NULL);
  GapBuffer *  pBuffer = pGapBufferManager->GetCurrent ();

  Location locCursor = pBuffer->GetCursor ();
  Location locSelect = pBuffer->GetSelection ();
  
  Location locBegin = locCursor;
  Location locEnd   = locSelect;
  
  if (locBegin > locEnd)
    {
    locBegin = locSelect;
    locEnd   = locCursor;
    }

  pBuffer->ClampLocationToValidChar (locBegin);
  pBuffer->ClampLocationToValidChar (locEnd);
    

  pBuffer->SetCursor (locBegin);
  INT  iCharsToDelete = pBuffer->GetCharsBetween (locBegin, locEnd);
  
  pBuffer->DeleteChars (iCharsToDelete);
  
  // undoable  
  
  };
  
  
//-----------------------------------------------------------------------------
VOID CmdSelectionCut (RStrArray *  arrayParams)
  {
  SelectionCopy ();
  SelectionDelete ();
  };
  
//-----------------------------------------------------------------------------
VOID CmdSelectionCopy (RStrArray *  arrayParams)
  {
  SelectionCopy ();
  };
  
//-----------------------------------------------------------------------------
VOID CmdSelectionPaste (RStrArray *  arrayParams)
  {
  };

//-----------------------------------------------------------------------------
VOID CmdSelectionDelete (RStrArray *  arrayParams)
  {
  SelectionDelete ();
  };
  
  