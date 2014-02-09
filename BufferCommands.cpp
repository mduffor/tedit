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

#include "RStrParser.hpp"
#include "Command.hpp"
#include "GapBuffer.hpp"
#include "GapBufferManager.hpp"
#include "BufferCommands.hpp"
#include "EditorSettings.hpp"
#include "EntryFieldHandler.hpp"


// debugging
#include <ncurses.h>


static GapBufferManager *   pGapBufferManager = NULL;
static GapBuffer *          pInputBuffer = NULL;
static GapBuffer *          pDisplayBuffer = NULL;
static FormatInfo *         pFormatInfo = NULL;
static EditorSettings *     pSettings = NULL;
static CommandManager *     pCmdManager = NULL;
static EntryFieldHandler *  pEntryFieldHandler = NULL; 

//-----------------------------------------------------------------------------
VOID InitBufferCommands (GapBufferManager *   pBufferManagerIn,
                         CommandManager &     cmdManagerIn,
                         FormatInfo *         pFormatIn,
                         EditorSettings *     pSettingsIn,
                         EntryFieldHandler *  pEntryFieldHandlerIn)
  {
  ASSERT (pBufferManagerIn != NULL);
  ASSERT (pFormatIn != NULL);
  ASSERT (pSettingsIn != NULL);
  
  pGapBufferManager  = pBufferManagerIn;
  pFormatInfo        = pFormatIn;
  pSettings          = pSettingsIn;
  pCmdManager        = &cmdManagerIn;
  pEntryFieldHandler = pEntryFieldHandlerIn;
  
  BufferCommandsSetBuffer (pGapBufferManager->GetCurrent (), pGapBufferManager->GetCurrent ());

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
  cmdManagerIn.AddCommand ("GotoLinePrompt", CmdGotoLinePrompt);
  cmdManagerIn.AddCommand ("GotoLine", CmdGotoLine);
  cmdManagerIn.AddCommand ("FindTextPrompt", CmdFindTextPrompt);
  cmdManagerIn.AddCommand ("FindText", CmdFindText);
  
  cmdManagerIn.AddCommand ("FindTextNext", CmdFindTextNext);
  cmdManagerIn.AddCommand ("FindTextPrev", CmdFindTextPrev);
  
  cmdManagerIn.AddCommand ("Save",    CmdSaveCurrentBuffer);
  cmdManagerIn.AddCommand ("SaveAll", CmdSaveAllBuffers);
  
  
  };

//-----------------------------------------------------------------------------
VOID BufferCommandsSetBuffer (GapBuffer *  pInputBufferIn,
                              GapBuffer *  pDisplayBufferIn)
  {
  pInputBuffer   = pInputBufferIn;
  pDisplayBuffer = pDisplayBufferIn;
  }
  
//-----------------------------------------------------------------------------
VOID StartSelection (VOID)
  {
  if (!pInputBuffer->IsSelectionValid ())
    {
    pInputBuffer->SetSelection (pInputBuffer->GetCursor ());
    };
  };
  
//-----------------------------------------------------------------------------
VOID DismissSelection (VOID)
  {
  pInputBuffer->ClearSelection ();
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
  if (!pInputBuffer->IsSelectionValid())
    {
    pInputBuffer->DeleteChars (-1);
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
  if (!pInputBuffer->IsSelectionValid())
    {
    pInputBuffer->DeleteChars (1);
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
  Location  locCursor = pInputBuffer->GetCursor ();
  if (locCursor.iLine > 1) {
    locCursor.iLine = TMax (locCursor.iLine - 1, 1);
    pInputBuffer->SetCursor (locCursor);
    };
  pInputBuffer->MoveWindowToCursor ();
  // not undoable  
  };

//-----------------------------------------------------------------------------
VOID CursorDown (VOID)
  {
  Location  locCursor = pInputBuffer->GetCursor ();
  locCursor.iLine = TMin (locCursor.iLine + 1, pInputBuffer->GetNumLines ());
  pInputBuffer->SetCursor (locCursor);
  pInputBuffer->MoveWindowToCursor ();
  // not undoable  
  };

//-----------------------------------------------------------------------------
VOID CursorLeft (VOID)
  {
  Location  locCursor = pInputBuffer->GetCursor ();
  if (locCursor.iCol > 0) {
    locCursor.iCol -= 1;
    }
  else 
    {
    // wrap to previous line
    if (locCursor.iLine > 1)
      {
      locCursor.iLine -= 1;
      locCursor.iCol = pInputBuffer->GetLineLength (locCursor.iLine);
      }
    };
  pInputBuffer->SetCursor (locCursor);
  pInputBuffer->MoveWindowToCursor ();
  // not undoable  
  };

//-----------------------------------------------------------------------------
VOID CursorRight (VOID)
  {
  Location  locCursor = pInputBuffer->GetCursor ();
  locCursor.iCol += 1;
  pInputBuffer->SetCursor (locCursor);
  pInputBuffer->MoveWindowToCursor ();
  // not undoable  
  };

//-----------------------------------------------------------------------------
VOID CursorStartDoc ()
  {
  pInputBuffer->SetCursor (1, 0);
  pInputBuffer->MoveWindowToCursor ();
  // not undoable  
  };

//-----------------------------------------------------------------------------
VOID CursorEndDoc (VOID)
  {
  INT  iLastLine = pInputBuffer->GetNumLines ();
  pInputBuffer->SetCursor (iLastLine, pInputBuffer->GetLineLength (iLastLine));
  pInputBuffer->MoveWindowToCursor ();
  // not undoable  
  };

//-----------------------------------------------------------------------------
VOID CursorStartLine (VOID)
  {
  Location  locCursor = pInputBuffer->GetCursor ();
  locCursor.iCol = 0;
  pInputBuffer->SetCursor (locCursor);
  pInputBuffer->MoveWindowToCursor ();
  // not undoable  
  };

//-----------------------------------------------------------------------------
VOID CursorEndLine (VOID)
  {
  Location  locCursor = pInputBuffer->GetCursor ();
  locCursor.iCol = pInputBuffer->GetLineLength (locCursor.iLine);
  pInputBuffer->SetCursor (locCursor);
  pInputBuffer->MoveWindowToCursor ();
  // not undoable  
  };

//-----------------------------------------------------------------------------
VOID CursorPageUp (VOID)
  {
  Location  locCursor = pInputBuffer->GetCursor ();
  if (locCursor.iLine > 1) {
    locCursor.iLine = TMax (locCursor.iLine - pInputBuffer->GetLinesPerPage (), 1);
    pInputBuffer->SetCursor (locCursor);
    };
  pInputBuffer->MoveWindowToCursor ();
  // not undoable  
  };

//-----------------------------------------------------------------------------
VOID CursorPageDown (VOID)
  {
  Location  locCursor = pInputBuffer->GetCursor ();
  locCursor.iLine = TMin (locCursor.iLine + pInputBuffer->GetLinesPerPage (), pInputBuffer->GetNumLines ());
  pInputBuffer->SetCursor (locCursor);
  pInputBuffer->MoveWindowToCursor ();
  // not undoable  
  };
  
//-----------------------------------------------------------------------------
VOID CursorNextWord (VOID)
  {
  Location  locCursor = pInputBuffer->GetCursor ();

  // if beyond buffer, do nothing
  if (locCursor.iLine > pInputBuffer->GetNumLines())
    {
    return;
    }
  
  // if at line end, go to line beginning
  INT  iCurrChar = pInputBuffer->GetCharAtLocation (locCursor);
  if (pFormatInfo->IsEOL (iCurrChar) || (iCurrChar == -1))
    {
    locCursor.iLine += 1;
    locCursor.iCol = 0;
    pInputBuffer->SetCursor (locCursor);
    return;
    }
  // move forward past any word characters you are on
  while (pFormatInfo->IsWordChar (pInputBuffer->GetCharAtLocation (locCursor))) 
    {
    locCursor.iCol += 1;
    };
  // move past any non-word characters, such as whitespace and punctuation  
  iCurrChar = pInputBuffer->GetCharAtLocation (locCursor);
  while ((!pFormatInfo->IsWordChar (iCurrChar)) && 
            (!pFormatInfo->IsEOL (iCurrChar)) && 
            (iCurrChar != -1))
    {
    locCursor.iCol += 1;
    iCurrChar = pInputBuffer->GetCharAtLocation (locCursor);
    }

  pInputBuffer->SetCursor (locCursor);
  pInputBuffer->MoveWindowToCursor ();
  // not undoable  
  };

//-----------------------------------------------------------------------------
VOID CursorPrevWord (VOID)
  {
  Location  locCursor = pInputBuffer->GetCursor ();

  // if beyond buffer, go to end
  if (locCursor.iLine > pInputBuffer->GetNumLines())
    {
    INT  iLastLine = pInputBuffer->GetNumLines ();
    pInputBuffer->SetCursor (iLastLine, pInputBuffer->GetLineLength (iLastLine));
    return;
    }
  
  // if at line beginning, go to prev line end
  if ((locCursor.iCol == 0) && (locCursor.iLine > 1))
    {
    --locCursor.iLine;
    locCursor.iCol = pInputBuffer->GetLineLength (locCursor.iLine);
    pInputBuffer->SetCursor (locCursor);
    return;
    }
    
  // move past any non-word characters, such as whitespace and punctuation  
  while ((locCursor.iCol != 0) &&
         (!pFormatInfo->IsWordChar (pInputBuffer->GetCharAtLocation (locCursor.iLine, locCursor.iCol - 1))))
    {
    --locCursor.iCol;
    }

  // move past any word characters you are on
  while ((locCursor.iCol != 0) &&
         (pFormatInfo->IsWordChar (pInputBuffer->GetCharAtLocation (locCursor.iLine, locCursor.iCol - 1))))
    {
    --locCursor.iCol;
    };
  pInputBuffer->SetCursor (locCursor);
  pInputBuffer->MoveWindowToCursor ();

  // not undoable  
  };

//-----------------------------------------------------------------------------
VOID CursorIndent (VOID)
  {
  // This function assumes that we are on the line created when the enter
  //  has been pressed.  We need to read the previous line(s) to find the
  //  indentation level and what character starts the previous line.  Then
  //  we either insert spaces to properly indent this line (if not a blank line)
  //  or move the cursor so new spaces are added once typing resumes.

  if (!pSettings->IndentOn ()) return;
  
  // First, find the indentation level
  INT  iIndentation = 0;
  
  Location  locCursor = pInputBuffer->GetCursor ();
  Location  locSearch = Location (locCursor.iLine - 1, 0);

  if (locCursor.iLine <= 1) return;
  
  // step backwards through lines
  do 
    {
    // step forwards through characters
    INT  iLineLength = pInputBuffer->GetLineLength (locSearch.iLine);
    INT  iLineIndent = 0;
    
    for (INT  iCol = 0; iCol < iLineLength; ++iCol)
      {
      INT  iChar = pInputBuffer->GetCharAtLocation (locSearch.iLine, iCol);
      if (iChar == ' ') 
        {
        ++iLineIndent;
        }
      else if (iChar == '\t')
        {
        iLineIndent += pSettings->GetTabSize ();
        }
      else
        {
        // found end of line
        if (pSettings->IndentOnBracket ())
          {
          if (pSettings->IsIndentOpenBracket (iChar))
            {
            iLineIndent += pSettings->GetTabSize ();
            };
          }
        else
          {
          if (pSettings->IsIndentCloseBracket (iChar))
            {
            iLineIndent = TMax (0, iLineIndent - pSettings->GetTabSize ());
            };
          };
          
        locCursor.Set (locCursor.iLine, iLineIndent);
        pInputBuffer->SetCursor (locCursor);
        return;
        }
      };
    locSearch.Set (locSearch.iLine - 1, 0);
    } while (locSearch.iLine > 0);
  }
  
  
//-----------------------------------------------------------------------------
VOID SelectionCopy (VOID)
  {
  Location locBegin = pInputBuffer->GetSelectionStart ();
  Location locEnd   = pInputBuffer->GetSelectionEnd ();
  pInputBuffer->ClampLocationToValidChar (locBegin);
  pInputBuffer->ClampLocationToValidChar (locEnd);

  // copy selected text to clip
  pInputBuffer->SetCursor (locBegin);
  INT  iCharsToCopy = pInputBuffer->GetCharsBetween (locBegin, locEnd) + 1;
  
  RStr &  strClip = pSettings->GetClip ();
  strClip.GrowAbsolute (iCharsToCopy + 1);
  strClip.SetAt (iCharsToCopy, '\0');

  pInputBuffer->GetString (strClip.GetBufferPtr (), iCharsToCopy);
  //printf ("Copied %d chars (%d, %d) to (%d, %d):\n", iCharsToCopy, locBegin.iLine, locBegin.iCol, locEnd.iLine, locEnd.iCol);
  //printf (strClip.AsChar ());
  //printf ("\n");
  
  // not undoable  
  };

//-----------------------------------------------------------------------------
VOID SelectionDelete (VOID)
  {
  if (!pInputBuffer->IsSelectionValid())
    {
    return;
    }
  
  Location locBegin = pInputBuffer->GetSelectionStart ();
  Location locEnd   = pInputBuffer->GetSelectionEnd ();
  pInputBuffer->ClampLocationToValidChar (locBegin);
  pInputBuffer->ClampLocationToValidChar (locEnd);

  pInputBuffer->SetCursor (locBegin);
  INT  iCharsToDelete = pInputBuffer->GetCharsBetween (locBegin, locEnd) + 1;
  
  //printf ("Deleting chars between %d,%d and %d,%d num chars %d\n", locBegin.iLine, locBegin.iCol, locEnd.iLine, locEnd.iCol, iCharsToDelete);
  
  pInputBuffer->DeleteChars (iCharsToDelete);
  pInputBuffer->ClearSelection ();
  pInputBuffer->MoveWindowToCursor ();
  
  // TODO: Undo
  // undoable  
  
  };

//-----------------------------------------------------------------------------
VOID SelectAll (VOID)  
  {
  INT  iLastLine = pInputBuffer->GetNumLines ();
  pInputBuffer->SetCursor(1, 0);
  pInputBuffer->SetSelection(iLastLine, pInputBuffer->GetLineLength (iLastLine));
  pInputBuffer->MoveWindowToCursor ();
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
  ScrollWindow (pInputBuffer->GetLinesPerPage ());
  }  
  
//-----------------------------------------------------------------------------
VOID ScrollWindowPageDown (VOID)
  {
  ScrollWindow (-pInputBuffer->GetLinesPerPage ());
  }  

//-----------------------------------------------------------------------------
VOID ScrollWindow (INT  iNumLinesIn)
  {
  Location  locWindowPosition = pInputBuffer->GetWindowPos ();
  locWindowPosition.iLine = TMax (1, TMin (pInputBuffer->GetNumLines(), locWindowPosition.iLine + iNumLinesIn));
  pInputBuffer->SetWindowPos (locWindowPosition);  
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
  pInputBuffer->InsertString(strClip.AsChar ());
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
  };
  
//-----------------------------------------------------------------------------
VOID CmdScrollWindowDown (RStrArray *  arrayParams)
  {
  ScrollWindowDown ();
  };
  
//-----------------------------------------------------------------------------
VOID CmdCursorPageUp (RStrArray *  arrayParams)
  {
  ScrollWindowPageDown ();
  CursorPageUp ();
  };
  
//-----------------------------------------------------------------------------
VOID CmdCursorPageDown (RStrArray *  arrayParams)
  {
  ScrollWindowPageUp ();
  CursorPageDown ();
  };  
  
//-----------------------------------------------------------------------------
VOID CmdGotoLinePrompt (RStrArray *  arrayParams)
  {
  RStrArray    astrInputFields;
  
  astrInputFields.SetLength (1);
  astrInputFields[0].Set ("Line");
  pEntryFieldHandler->StartInput ("GotoLine", astrInputFields);  
  };  
    
//-----------------------------------------------------------------------------
VOID CmdGotoLine (RStrArray *  arrayParams)
  {
  ASSERT (arrayParams != NULL);
  ASSERT (arrayParams->Length () == 1);
  
  RStrParser  parser ((*arrayParams)[0]);
  INT iLine = parser.GetInt ();
  
  Location  locCursor = pDisplayBuffer->GetCursor ();
  locCursor.iLine = iLine;
  pDisplayBuffer->ClampLocationToValidChar (locCursor);
  pDisplayBuffer->SetCursor (locCursor);
  pDisplayBuffer->MoveWindowToCursor ();
  };  
  
//-----------------------------------------------------------------------------
VOID CmdFindTextPrompt (RStrArray *  arrayParams)
  {
  RStrArray    astrInputFields;
  
  astrInputFields.SetLength (1);
  astrInputFields[0].Set ("Find");
  pEntryFieldHandler->StartInput ("FindText", astrInputFields);  
  };  
    
//-----------------------------------------------------------------------------
VOID CmdFindText (RStrArray *  arrayParams)
  {
  ASSERT (arrayParams != NULL);
  ASSERT (arrayParams->Length () >= 1);

  pDisplayBuffer->FindAll ((*arrayParams)[0].AsChar());
  pDisplayBuffer->CursorToFindNext (pSettings->IsWrapSearchMode ());
  pDisplayBuffer->MoveWindowToCursor ();
  };  

//-----------------------------------------------------------------------------
VOID CmdFindTextNext (RStrArray *  arrayParams)
  {
  pDisplayBuffer->FindAll (pDisplayBuffer->GetSearchTerm ());
  pDisplayBuffer->CursorToFindNext (pSettings->IsWrapSearchMode ());
  pDisplayBuffer->MoveWindowToCursor ();
  };  
  
//-----------------------------------------------------------------------------
VOID CmdFindTextPrev (RStrArray *  arrayParams)
  {
  pDisplayBuffer->FindAll (pDisplayBuffer->GetSearchTerm ());
  pDisplayBuffer->CursorToFindPrev (pSettings->IsWrapSearchMode ());
  pDisplayBuffer->MoveWindowToCursor ();
  };  
  
//-----------------------------------------------------------------------------
VOID CmdSaveCurrentBuffer (RStrArray *  arrayParams)
  {
  
  GapBuffer *  pCurr = pGapBufferManager->GetCurrent ();
  if (pCurr != NULL)
    {
    RStr  strFileName (pCurr->GetFileName());
    strFileName.StripTrailingWhitespace();
    if (strFileName.IsEmpty ())
      {
      // TODO: Handle error condition where file name is not defined.
      }
    else
      {
      pCurr->Save ();
      }
    }
  };  
  
//-----------------------------------------------------------------------------
VOID CmdSaveAllBuffers (RStrArray *  arrayParams)
  {

  GapBuffer *  pCurr = pGapBufferManager->GetFirstBuffer ();
  while (pCurr != NULL)
    {
    if (pCurr->GetIsModified ())
      {
      RStr  strFileName (pCurr->GetFileName());
      strFileName.StripTrailingWhitespace();
      if (strFileName.IsEmpty ())
        {
        // TODO: Handle error condition where file name is not defined.
        }
      else
        {
        pCurr->Save ();
        }
      }
    pCurr = pGapBufferManager->GetNextBuffer (pCurr);
    };
  };  
  
  
  
  
  
  
  
  
  
  
  