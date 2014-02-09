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


VOID CmdSelectionCut     (RStrArray *  arrayParams = NULL);
VOID CmdSelectionCopy    (RStrArray *  arrayParams = NULL);
VOID CmdSelectionPaste   (RStrArray *  arrayParams = NULL);
VOID CmdSelectionDelete  (RStrArray *  arrayParams = NULL);
VOID CmdSelectAll        (RStrArray *  arrayParams = NULL);
VOID CmdScrollWindowUp   (RStrArray *  arrayParams = NULL);
VOID CmdScrollWindowDown (RStrArray *  arrayParams = NULL);
VOID CmdCursorPageUp     (RStrArray *  arrayParams = NULL);
VOID CmdCursorPageDown   (RStrArray *  arrayParams = NULL);

VOID CmdGotoLinePrompt (RStrArray *  arrayParams = NULL);
VOID CmdGotoLine       (RStrArray *  arrayParams = NULL);
VOID CmdFindTextPrompt (RStrArray *  arrayParams = NULL);
VOID CmdFindText       (RStrArray *  arrayParams = NULL);

VOID CmdFindTextNext (RStrArray *  arrayParams = NULL);
VOID CmdFindTextPrev (RStrArray *  arrayParams = NULL);

VOID CmdSaveCurrentBuffer (RStrArray *  arrayParams = NULL);
VOID CmdSaveAllBuffers    (RStrArray *  arrayParams = NULL);



#endif // BUFFERCOMMANDS_HPP