//
// BufferCommands.hpp
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

#ifndef BUFFERCOMMANDS_HPP
#define BUFFERCOMMANDS_HPP

#include "GapBufferManager.hpp"
#include "Command.hpp"
#include "FormatInfo.hpp"
#include "EditorSettings.hpp"
#include "EntryFieldHandler.hpp"

VOID InitBufferCommands (GapBufferManager *   pManagerIn,
                         CommandManager &     cmdManagerIn,
                         FormatInfo *         pFormatIn,
                         EditorSettings *     pSettings,
                         EntryFieldHandler *  pEntryFieldHandlerIn);

VOID BufferCommandsSetBuffer (GapBuffer *  pInputBufferIn,
                              GapBuffer *  pDisplayBufferIn);

VOID CmdCursorUp (RStrArray *  arrayParams);
VOID CmdCursorDown (RStrArray *  arrayParams);
VOID CmdCursorLeft (RStrArray *  arrayParams);
VOID CmdCursorRight (RStrArray *  arrayParams);
VOID CmdCursorStartDoc (RStrArray *  arrayParams);
VOID CmdCursorEndDoc (RStrArray *  arrayParams);
VOID CmdCursorStartLine (RStrArray *  arrayParams);
VOID CmdCursorEndLine (RStrArray *  arrayParams);
VOID CmdCursorNextWord (RStrArray *  arrayParams);
VOID CmdCursorPrevWord (RStrArray *  arrayParams);

VOID CmdSelectUp (RStrArray *  arrayParams);
VOID CmdSelectDown (RStrArray *  arrayParams);
VOID CmdSelectLeft (RStrArray *  arrayParams);
VOID CmdSelectRight (RStrArray *  arrayParams);
VOID CmdSelectStartDoc (RStrArray *  arrayParams);
VOID CmdSelectEndDoc (RStrArray *  arrayParams);
VOID CmdSelectStartLine (RStrArray *  arrayParams);
VOID CmdSelectEndLine (RStrArray *  arrayParams);
VOID CmdSelectNextWord (RStrArray *  arrayParams);
VOID CmdSelectPrevWord (RStrArray *  arrayParams);

VOID CmdBackspace (RStrArray *  arrayParams);
VOID CmdDelete    (RStrArray *  arrayParams);

VOID CursorUp (VOID);
VOID CursorDown (VOID);
VOID CursorLeft (VOID);
VOID CursorRight (VOID);
VOID CursorStartDoc (VOID);
VOID CursorEndDoc (VOID);
VOID CursorStartLine (VOID);
VOID CursorEndLine (VOID);
VOID CursorNextWord (VOID);
VOID CursorPrevWord (VOID);
VOID CursorIndent (VOID);
VOID CursorPageUp (VOID);
VOID CursorPageDown (VOID);
  
VOID SelectionCopy (VOID);
VOID SelectionDelete (VOID);
VOID SelectAll (VOID);
VOID ScrollWindowUp (VOID);
VOID ScrollWindowDown (VOID);
VOID ScrollWindowPageUp (VOID);
VOID ScrollWindowPageDown (VOID);
VOID ScrollWindow (INT  iNumLinesIn);


VOID CmdSelectionCut (RStrArray *  arrayParams);
VOID CmdSelectionCopy (RStrArray *  arrayParams);
VOID CmdSelectionPaste (RStrArray *  arrayParams);
VOID CmdSelectionDelete (RStrArray *  arrayParams);
VOID CmdSelectAll (RStrArray *  arrayParams);
VOID CmdScrollWindowUp (RStrArray *  arrayParams);
VOID CmdScrollWindowDown (RStrArray *  arrayParams);
VOID CmdCursorPageUp (RStrArray *  arrayParams);
VOID CmdCursorPageDown (RStrArray *  arrayParams);

VOID CmdGotoLinePrompt (RStrArray *  arrayParams);
VOID CmdGotoLine (RStrArray *  arrayParams);
VOID CmdFindTextPrompt (RStrArray *  arrayParams);
VOID CmdFindText (RStrArray *  arrayParams);

VOID CmdFindTextNext (RStrArray *  arrayParams);
VOID CmdFindTextPrev (RStrArray *  arrayParams);

#endif // BUFFERCOMMANDS_HPP