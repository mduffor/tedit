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
static GapBuffer *        pBuffer = NULL;
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
  BufferCommandsSetBuffer (pGapBufferManager->GetCurrent ());


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
  cmdManagerIn.AddCommand ("CursorPageDown", CmdCursorPageDown);
  cmdManagerIn.AddCommand ("CursorPageUp", CmdCursorPageUp);
  
  cmdManagerIn.AddCommand ("SelectUp", CmdSelectUp);
  cmdManagerIn.AddCommand ("SelectDown", CmdSelectDown);
  cmdManagerIn.AddCommand ("SelectLeft", CmdSelectLeft);
  cmdManagerIn.AddCommand ("SelectRight", CmdSelectRight);
  
  cmdManagerIn.AddCommand ("SelectNextWord", CmdSelectNextWord);
  cmdManagerIn.AddCommand ("SelectPrevWord", CmdSelectPrevWord);
  cmdManagerIn.AddCommand ("SelectStartLine", CmdSelectStartLine);
  cmdManagerIn.AddCommand ("SelectEndLine", CmdSelectEndLine);
  cmdManagerIn.AddCommand ("SelectStartDoc", CmdSelectStartDoc);
  cmdManagerIn.AddCommand ("SelectEndDoc", CmdSelectEndDoc);
  
  cmdManagerIn.AddCommand ("SelectionCut", CmdSelectionCut);
  cmdManagerIn.AddCommand ("SelectionCopy", CmdSelectionCopy);
  cmdManagerIn.AddCommand ("SelectionPaste", CmdSelectionPaste);
  cmdManagerIn.AddCommand ("SelectionDelete", CmdSelectionDelete);
  cmdManagerIn.AddCommand ("SelectAll", CmdSelectAll);
  cmdManagerIn.AddCommand ("ScrollWindowUp", CmdScrollWindowUp);
  cmdManagerIn.AddCommand ("ScrollWindowDown", CmdScrollWindowDown);
  
  cmdManagerIn.AddCommand ("Backspace", CmdBackspace);
  cmdManagerIn.AddCommand ("Delete", CmdDelete);
  };

//-----------------------------------------------------------------------------
VOID BufferCommandsSetBuffer (GapBuffer *  pBufferIn)
  {
  pBuffer = pBufferIn;
  }
  
//-----------------------------------------------------------------------------
VOID StartSelection (VOID)
  {
  if (!pBuffer->IsSelectionValid ())
    {
    pBuffer->SetSelection (pBuffer->GetCursor ());
    };
  };
  
//-----------------------------------------------------------------------------
VOID DismissSelection (VOID)
  {
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
VOID CmdBackspace (RStrArray *  arrayParams)
  {
  if (!pBuffer->IsSelectionValid())
    {
    pBuffer->DeleteChars (-1);
    // TODO: Undo
    }
  else
    {
    SelectionDelete();
    }
  };
  
//-----------------------------------------------------------------------------
VOID CmdDelete (RStrArray *  arrayParams)
  {
  if (!pBuffer->IsSelectionValid())
    {
    pBuffer->DeleteChars (1);
    // TODO: Undo
    }
  else
    {
    SelectionDelete();
    }
  };

//-----------------------------------------------------------------------------
VOID CursorUp (VOID)
  {
  Location  locCursor = pBuffer->GetCursor ();
  if (locCursor.iLine > 1) {
    locCursor.iLine = TMax (locCursor.iLine - 1, 1);
    pBuffer->SetCursor (locCursor);
    };
  pBuffer->MoveWindowToCursor ();
  // not undoable  
  };

//-----------------------------------------------------------------------------
VOID CursorDown (VOID)
  {
  Location  locCursor = pBuffer->GetCursor ();
  locCursor.iLine = TMin (locCursor.iLine + 1, pBuffer->GetNumLines ());
  pBuffer->SetCursor (locCursor);
  pBuffer->MoveWindowToCursor ();
  // not undoable  
  };

//-----------------------------------------------------------------------------
VOID CursorLeft (VOID)
  {
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
  pBuffer->MoveWindowToCursor ();
  // not undoable  
  };

//-----------------------------------------------------------------------------
VOID CursorRight (VOID)
  {
  Location  locCursor = pBuffer->GetCursor ();
  locCursor.iCol += 1;
  pBuffer->SetCursor (locCursor);
  pBuffer->MoveWindowToCursor ();
  // not undoable  
  };

//-----------------------------------------------------------------------------
VOID CursorStartDoc ()
  {
  pBuffer->SetCursor (1, 0);
  pBuffer->MoveWindowToCursor ();
  // not undoable  
  };

//-----------------------------------------------------------------------------
VOID CursorEndDoc (VOID)
  {
  INT  iLastLine = pBuffer->GetNumLines ();
  pBuffer->SetCursor (iLastLine, pBuffer->GetLineLength (iLastLine));
  pBuffer->MoveWindowToCursor ();
  // not undoable  
  };

//-----------------------------------------------------------------------------
VOID CursorStartLine (VOID)
  {
  Location  locCursor = pBuffer->GetCursor ();
  locCursor.iCol = 0;
  pBuffer->SetCursor (locCursor);
  pBuffer->MoveWindowToCursor ();
  // not undoable  
  };

//-----------------------------------------------------------------------------
VOID CursorEndLine (VOID)
  {
  Location  locCursor = pBuffer->GetCursor ();
  locCursor.iCol = pBuffer->GetLineLength (locCursor.iLine);
  pBuffer->SetCursor (locCursor);
  pBuffer->MoveWindowToCursor ();
  // not undoable  
  };

//-----------------------------------------------------------------------------
VOID CursorPageUp (VOID)
  {
  Location  locCursor = pBuffer->GetCursor ();
  if (locCursor.iLine > 1) {
    locCursor.iLine = TMax (locCursor.iLine - pBuffer->GetLinesPerPage (), 1);
    pBuffer->SetCursor (locCursor);
    };
  pBuffer->MoveWindowToCursor ();
  // not undoable  
  };

//-----------------------------------------------------------------------------
VOID CursorPageDown (VOID)
  {
  Location  locCursor = pBuffer->GetCursor ();
  locCursor.iLine = TMin (locCursor.iLine + pBuffer->GetLinesPerPage (), pBuffer->GetNumLines ());
  pBuffer->SetCursor (locCursor);
  pBuffer->MoveWindowToCursor ();
  // not undoable  
  };
  
//-----------------------------------------------------------------------------
VOID CursorNextWord (VOID)
  {
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
  pBuffer->MoveWindowToCursor ();
  // not undoable  
  };

//-----------------------------------------------------------------------------
VOID CursorPrevWord (VOID)
  {
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

  // move past any word characters you are on
  while ((locCursor.iCol != 0) &&
         (pFormatInfo->IsWordChar (pBuffer->GetCharAtLocation (locCursor.iLine, locCursor.iCol - 1))))
    {
    --locCursor.iCol;
    };
  pBuffer->SetCursor (locCursor);
  pBuffer->MoveWindowToCursor ();

  // not undoable  
  };

//-----------------------------------------------------------------------------
VOID SelectionCopy (VOID)
  {
  Location locBegin = pBuffer->GetSelectionStart ();
  Location locEnd   = pBuffer->GetSelectionEnd ();
  pBuffer->ClampLocationToValidChar (locBegin);
  pBuffer->ClampLocationToValidChar (locEnd);

  // copy selected text to clip
  pBuffer->SetCursor (locBegin);
  INT  iCharsToCopy = pBuffer->GetCharsBetween (locBegin, locEnd) + 1;
  
  RStr &  strClip = pSettings->GetClip ();
  strClip.GrowAbsolute (iCharsToCopy + 1);
  strClip.SetAt (iCharsToCopy, '\0');

  pBuffer->GetString (strClip.GetBufferPtr (), iCharsToCopy);
  //printf ("Copied %d chars (%d, %d) to (%d, %d):\n", iCharsToCopy, locBegin.iLine, locBegin.iCol, locEnd.iLine, locEnd.iCol);
  //printf (strClip.AsChar ());
  //printf ("\n");
  
  // not undoable  
  };

//-----------------------------------------------------------------------------
VOID SelectionDelete (VOID)
  {
  if (!pBuffer->IsSelectionValid())
    {
    return;
    }
  
  Location locBegin = pBuffer->GetSelectionStart ();
  Location locEnd   = pBuffer->GetSelectionEnd ();
  pBuffer->ClampLocationToValidChar (locBegin);
  pBuffer->ClampLocationToValidChar (locEnd);

  pBuffer->SetCursor (locBegin);
  INT  iCharsToDelete = pBuffer->GetCharsBetween (locBegin, locEnd) + 1;
  
  //printf ("Deleting chars between %d,%d and %d,%d num chars %d\n", locBegin.iLine, locBegin.iCol, locEnd.iLine, locEnd.iCol, iCharsToDelete);
  
  pBuffer->DeleteChars (iCharsToDelete);
  pBuffer->ClearSelection ();
  pBuffer->MoveWindowToCursor ();
  
  // TODO: Undo
  // undoable  
  
  };

//-----------------------------------------------------------------------------
VOID SelectAll (VOID)  
  {
  INT  iLastLine = pBuffer->GetNumLines ();
  pBuffer->SetCursor(1, 0);
  pBuffer->SetSelection(iLastLine, pBuffer->GetLineLength (iLastLine));
  pBuffer->MoveWindowToCursor ();
  };

//-----------------------------------------------------------------------------
VOID ScrollWindowUp (VOID)
  {
  ScrollWindow (1);
  }
  
//-----------------------------------------------------------------------------
VOID ScrollWindowDown (VOID)
  {
  ScrollWindow (-1);
  }
  
//-----------------------------------------------------------------------------
VOID ScrollWindowPageUp (VOID)
  {
  ScrollWindow (pBuffer->GetLinesPerPage ());
  }  
  
//-----------------------------------------------------------------------------
VOID ScrollWindowPageDown (VOID)
  {
  ScrollWindow (-pBuffer->GetLinesPerPage ());
  }  

//-----------------------------------------------------------------------------
VOID ScrollWindow (INT  iNumLinesIn)
  {
  Location  locWindowPosition = pBuffer->GetWindowPos ();
  locWindowPosition.iLine = TMax (1, TMin (pBuffer->GetNumLines(), locWindowPosition.iLine + iNumLinesIn));
  pBuffer->SetWindowPos (locWindowPosition);  
  }  
  
  
//-----------------------------------------------------------------------------
VOID CmdSelectionCut (RStrArray *  arrayParams)
  {
  SelectionCopy ();
  SelectionDelete ();
  // TODO: Undo
  };
  
//-----------------------------------------------------------------------------
VOID CmdSelectionCopy (RStrArray *  arrayParams)
  {
  SelectionCopy ();
  // TODO: Undo
  };
  
//-----------------------------------------------------------------------------
VOID CmdSelectionPaste (RStrArray *  arrayParams)
  {
  SelectionDelete ();
  RStr &  strClip = pSettings->GetClip ();
  pBuffer->InsertString(strClip.AsChar ());
  // TODO: Undo
  };                   

//-----------------------------------------------------------------------------
VOID CmdSelectionDelete (RStrArray *  arrayParams)
  {
  SelectionDelete ();
  // TODO: Undo
  };
  
//-----------------------------------------------------------------------------
VOID CmdSelectAll (RStrArray *  arrayParams)
  {
  SelectAll ();
  // TODO: Undo
  };
  
//-----------------------------------------------------------------------------
VOID CmdScrollWindowUp (RStrArray *  arrayParams)
  {
  ScrollWindowUp ();
  // TODO: Undo
  };
  
//-----------------------------------------------------------------------------
VOID CmdScrollWindowDown (RStrArray *  arrayParams)
  {
  ScrollWindowDown ();
  // TODO: Undo
  };
  
//-----------------------------------------------------------------------------
VOID CmdCursorPageUp (RStrArray *  arrayParams)
  {
  ScrollWindowPageDown ();
  CursorPageUp ();
  // TODO: Undo
  };
  
//-----------------------------------------------------------------------------
VOID CmdCursorPageDown (RStrArray *  arrayParams)
  {
  ScrollWindowPageUp ();
  CursorPageDown ();
  // TODO: Undo
  };  
  
  
  
  