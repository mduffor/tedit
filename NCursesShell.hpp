//
// NCursesShell.hpp
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

#ifndef NCURSESSHELL_HPP
#define NCURSESSHELL_HPP

#include "Types.hpp"
#include "RStrArray.hpp"
#include "IntArray.hpp"
#include "GapBuffer.hpp"
#include "Command.hpp"
#include "EditorSettings.hpp"
#include "EntryFieldHandler.hpp"

//=============================================================================
class NCursesShell
  {
  private:
  
    BOOL         bShowLineNumbers;
    BOOL         bShowStatusBar;
    BOOL         bShowFileList;
    INT          iFileListWidth;
    
    RStrArray    astrFileList;
    IntArray     aiFileLoaded;
    INT          iFileListTopLine;
    INT          iFileListCursor;
    INT          iFileListHeight;
    
    
  public:
    NCursesShell  ();
    ~NCursesShell ();
    
    VOID Update                (GapBuffer *          pBuffer,
                                CommandManager &     cmdManager,
                                EditorSettings &     editorSettings,
                                EntryFieldHandler &  entryFieldHandler);

    VOID DisplayFileList       (INT                  iScreenX, 
                                INT                  iScreenY,
                                INT                  iWidth,
                                INT                  iHeight);

    VOID KeepFileListCursorOnScreen (VOID);
                               
    VOID FileListCursorUp      (VOID);

    VOID FileListCursorDown    (VOID);
    
    VOID ToggleFileListDisplay (VOID);
    
    BOOL ProcessInputFileList (CommandManager &     cmdManager,
                               EditorSettings &     editorSettings);
                               
                               
                               
    INT  NumDigits             (INT  iValueIn);

    VOID DisplayWindow         (INT                  iScreenX, 
                                INT                  iScreenY,
                                INT                  iWidth,
                                INT                  iHeight, 
                                GapBuffer *          pBuffer,
                                EditorSettings &     editorSettings,
                                EntryFieldHandler &  entryFieldHandler);
 
    BOOL ProcessInput          (GapBuffer *          pInputBuffer,
                                GapBuffer *          pDisplayBuffer,
                                CommandManager &     cmdManager,
                                EditorSettings &     editorSettings,
                                EntryFieldHandler &  entryFieldHandler);
                             
    VOID DrawBufferEntryField  (INT               iScreenX, 
                                INT               iScreenY,
                                INT               iWidth,
                                GapBuffer &       buffer,
                                EditorSettings &  editorSettings);
                                          
    BOOL GetHighlightState     (Location &  locCurr,
                                Location &  locBegin,
                                Location &  locEnd);

    INT  NCursesToVKey         (INT  nCursesKey);

    
  };  
  

#endif // NCURSESSHELL_HPP