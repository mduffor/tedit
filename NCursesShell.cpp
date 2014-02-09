//
// NCursesShell.cpp
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
#include <string.h>
#include "Types.hpp"
#include "Debug.hpp"
ASSERTFILE (__FILE__)

#include <ncurses.h>
#include "NCursesShell.hpp"
#include "GapBuffer.hpp"
#include "SyntaxParser.hpp"
#include "VKeys.hpp"
#include "BufferCommands.hpp"
#include "EntryFieldHandler.hpp"
#include "FilePath.hpp"

//-----------------------------------------------------------------------------
NCursesShell::NCursesShell ()
  {
  bShowLineNumbers = TRUE;
  bShowStatusBar = TRUE;
  bShowFileList = TRUE;
  iFileListWidth = 20;
  
  // file list init.  Move to own class
  iFileListTopLine = 0;
  iFileListCursor = 1;
  
  astrFileList.Append ("./SyntaxTest.txt");       aiFileLoaded.Append (1);
  astrFileList.Append ("./test/");                aiFileLoaded.Append (0);
  astrFileList.Append ("./test/helloWorld.txt");  aiFileLoaded.Append (1);
  astrFileList.Append ("./test/helloWorld2.txt"); aiFileLoaded.Append (1);
  astrFileList.Append ("./test/helloWorld3.txt"); aiFileLoaded.Append (0);
  astrFileList.Append ("./test/helloWorld4.txt"); aiFileLoaded.Append (0);
  astrFileList.Append ("./test/helloWorld5.txt"); aiFileLoaded.Append (0);
  astrFileList.Append ("./test/helloWorld6.txt"); aiFileLoaded.Append (0);
  astrFileList.Append ("./test/helloWorld7.txt"); aiFileLoaded.Append (0);
  astrFileList.Append ("./test/helloWorld8.txt"); aiFileLoaded.Append (0);
  astrFileList.Append ("./test/helloWorld9.txt"); aiFileLoaded.Append (0);
  astrFileList.Append ("./test/helloWorld10.txt"); aiFileLoaded.Append (0);
  astrFileList.Append ("./test/helloWorld11.txt"); aiFileLoaded.Append (0);
  astrFileList.Append ("./test/helloWorld12.txt"); aiFileLoaded.Append (0);
  astrFileList.Append ("./test/helloWorld13.txt"); aiFileLoaded.Append (0);
  astrFileList.Append ("./test/helloWorld14.txt"); aiFileLoaded.Append (0);
  astrFileList.Append ("./test/helloWorld15.txt"); aiFileLoaded.Append (0);
  astrFileList.Append ("./test/helloWorld16.txt"); aiFileLoaded.Append (0);
  astrFileList.Append ("./test/helloWorld17.txt"); aiFileLoaded.Append (0);
  astrFileList.Append ("./test/helloWorld18.txt"); aiFileLoaded.Append (0);
  astrFileList.Append ("./test/helloWorld19.txt"); aiFileLoaded.Append (0);
  
  initscr ();  // start ncurses
  raw ();      // disable line buffering
  keypad (stdscr, TRUE); // enable function keys
  noecho ();             // don't echo characters; we will display them ourself
  
  INT  iBGColor = COLOR_BLACK;
  start_color ();
  init_pair (1, COLOR_RED, iBGColor);
  init_pair (2, COLOR_GREEN, iBGColor);
  init_pair (3, COLOR_YELLOW, iBGColor);
  init_pair (4, COLOR_BLUE, iBGColor);
  init_pair (5, COLOR_MAGENTA, iBGColor);
  init_pair (6, COLOR_CYAN, iBGColor);
  init_pair (7, COLOR_WHITE, iBGColor);
  
  };          

//-----------------------------------------------------------------------------
NCursesShell::~NCursesShell ()
  {
  endwin ();  // end ncurses mode
  };

//-----------------------------------------------------------------------------
VOID NCursesShell::Update (GapBufferManager *   pBufferManager,
                           CommandManager &     cmdManager,
                           EditorSettings &     editorSettings,
                           EntryFieldHandler &  entryFieldHandler)
  {
  int  iRowMax = 0;
  int  iColMax = 0;

  
  /*  
  printw ("Testing ncurses (%dx%d).  Press a key to see it in bold. \n", iColMax, iRowMax);
  int ch = getch();
  if (ch == KEY_F(1))
    {
    printw ("F1 key pressed\n");
    }
  else
    {
    printw ("The key pressed is ");
    attron(A_BOLD);
    printw ("%c", ch);
    attroff(A_BOLD);
    }
 */  

  
  //move(0,0);
  // addch (ch | A_BOLD | A_UNDERLINE);
  
  BOOL  bExit = FALSE;
  while (!bExit) 
    {
    getmaxyx (stdscr, iRowMax, iColMax);
    GapBuffer *  pBuffer = pBufferManager->GetCurrent();
    if (pBuffer == NULL)
      {
      // no buffer.  Error
      return;
      };
    
    INT  iHeader = 5;
    INT  iLeft = 0;

    if (bShowFileList)
      {
      iLeft = iFileListWidth;
      };
    
    INT  iStartX = 0;
    INT  iStartY = 0;
    INT  iWidth = iColMax;
    INT  iHeight = iRowMax;
    
    GapBuffer *  pBufferInputFocus = pBuffer;
    
    if (entryFieldHandler.IsActive ())
      {
      pBufferInputFocus = &(entryFieldHandler.GetBuffer ());
      }

    DisplayWindow (iStartX + iLeft, iStartY + iHeader,
                   iWidth - iLeft, iHeight - iHeader, // iColMax, iRowMax, 
                   pBuffer,
                   editorSettings,
                   entryFieldHandler);
    //refresh();
    if (bShowFileList)
      {
      DisplayFileList (pBufferManager, iStartX, iStartY + iHeader, iLeft, iHeight - iHeader);
      bExit = ProcessInputFileList (pBufferManager, cmdManager, editorSettings);
      }
    else 
      {
      bExit = ProcessInput (pBufferInputFocus, pBuffer, cmdManager, editorSettings, entryFieldHandler);
      }
    move(2,0);
    printw("file: %s                             ", pBuffer->GetName());
      
      
    refresh();
    }
  };

//-----------------------------------------------------------------------------
VOID NCursesShell::DisplayFileList (GapBufferManager *   pBufferManager,
                                    INT                  iScreenX, 
                                    INT                  iScreenY,
                                    INT                  iWidth,
                                    INT                  iHeight)
  {
  // This will eventually go into its own class
  
  iFileListHeight = iHeight;
  
  INT  iMaxLines = astrFileList.Length ();
  INT  iLine     = iFileListTopLine;
  INT  iRemaining = 0;
  INT  iIndex;
  for (iIndex = 0; iIndex < iHeight; ++iIndex)
    {
    if (iLine >= iMaxLines) break;
    
    //attron(A_BOLD);
    move (iScreenY + iIndex, iScreenX);
    INT  iStringLength = astrFileList[iLine].GetLength ();
    if (aiFileLoaded [iLine])
      {
      attrset(COLOR_PAIR (COLOR_GREEN));
      };
    if (iFileListCursor == iLine)
      {
      attron (A_REVERSE);
      };
    // TODO: display path indentation 
    
    // display modified flag
    INT  iModifiedFlagWidth = 0;
    if (aiFileLoaded [iLine])
      {
      GapBuffer *  pCurr = pBufferManager->GetBuffer (astrFileList[iLine].AsChar ());
      if (pCurr != NULL)
        {
        if (pCurr->GetIsModified ())
          {
          addch ('*');
          iModifiedFlagWidth = 1;
          }
        }
      }
    
    // display file name
    addnstr (astrFileList[iLine].AsChar (), TMin (iWidth, iStringLength));
    iRemaining = iWidth - iStringLength - iModifiedFlagWidth;
    while (iRemaining > 0)
      {
      addch (' ');
      --iRemaining;
      };
    ++iLine;
    attrset(A_NORMAL);
    }
    
  for (; iIndex < iHeight; ++iIndex)
    {
    move (iScreenY + iIndex, iScreenX);
    iRemaining = iWidth;
    while (iRemaining > 0)
      {
      addch (' ');
      --iRemaining;
      };
    }
  }

//-----------------------------------------------------------------------------
VOID NCursesShell::KeepFileListCursorOnScreen (VOID)
  {
  if (iFileListCursor < iFileListTopLine)
    {
    iFileListTopLine = iFileListCursor;
    }
  if (iFileListCursor >= iFileListTopLine + iFileListHeight)
    {
    iFileListTopLine = TMax (0, iFileListCursor - iFileListHeight + 1); 
    }
  }

//-----------------------------------------------------------------------------
VOID NCursesShell::FileListCursorUp (VOID)
  {
  // TODO: Handle collapsed sections
  iFileListCursor = TMax (0, TMin (astrFileList.Length () - 1, iFileListCursor - 1));
  KeepFileListCursorOnScreen ();
  }
  
//-----------------------------------------------------------------------------
VOID NCursesShell::FileListCursorDown (VOID)
  {
  // TODO: Handle collapsed sections
  iFileListCursor = TMax (0, TMin (astrFileList.Length () - 1, iFileListCursor + 1));
  KeepFileListCursorOnScreen ();
  }

//-----------------------------------------------------------------------------
VOID NCursesShell::ToggleFileListDisplay (VOID)
  {
  bShowFileList = bShowFileList ? FALSE : TRUE;
  }
  
//-----------------------------------------------------------------------------
BOOL NCursesShell::ProcessInputFileList (GapBufferManager *   pBufferManager,
                                         CommandManager &     cmdManager,
                                         EditorSettings &     editorSettings)
  {
  INT vKey = NCursesToVKey(getch());
  
  if (vKey >= 32 && vKey <= 126) 
    {
    // standard printable ASCII key
    // TODO: seek out files in directory that match typed letters
    }
  else
    {
    // Note: This needs to be converted to a configureable hotkey lookup.
    switch (vKey)
      {
      case VKEY_DOWN:      FileListCursorDown ();  break;
      case VKEY_UP:        FileListCursorUp ();  break;
      case VKEY_PAGEDOWN:  break;
      case VKEY_PAGEUP:    break;
      case VKEY_LEFT:      break;
      case VKEY_RIGHT:     break;
      case VKEY_HOME:      break;
      case VKEY_END:       break;
      
      case VKEY_ENTER:
        {
        // select if already open
        GapBuffer *  pExisting = pBufferManager->GetBufferByFileName (astrFileList[iFileListCursor]);
        if (pExisting != NULL)
          {
          // found it
          pBufferManager->SetCurrent(pExisting);
          ToggleFileListDisplay ();
          break;
          }
        // not already open.  see if it is a file.
        FilePath  filePath;
        if (filePath.FileExists (astrFileList[iFileListCursor]))
          {
          // open file
          
          RStrArray  arrayFileNameParts;
          FilePath::SplitPath (astrFileList[iFileListCursor], '/', arrayFileNameParts);
          
          GapBuffer *  pBuffer = pBufferManager->CreateBuffer (arrayFileNameParts[arrayFileNameParts.Length() - 1]);
          pBufferManager->SetCurrent (pBuffer);
          pBuffer->AllocBuffer (256);
          pBuffer->SetFileName (astrFileList[iFileListCursor]);
          pBuffer->Load ();
          pBuffer->FillGap ();
          pBuffer->SetCursor(1, 0);          
          ToggleFileListDisplay ();
    move(3,0);
    printw("current : %s                             ",  pBufferManager->GetCurrent()->GetName());
          
          }
        }
        break;
        
      case (CTRL_P):       ToggleFileListDisplay (); break;
      case (CTRL_Q):
        return (TRUE);
      };
    };
  return (FALSE);
  };
  
//-----------------------------------------------------------------------------
BOOL NCursesShell::ProcessInput (GapBuffer *          pInputBuffer,
                                 GapBuffer *          pDisplayBuffer,
                                 CommandManager &     cmdManager,
                                 EditorSettings &     editorSettings,
                                 EntryFieldHandler &  entryFieldHandler)
  {
  
  BufferCommandsSetBuffer (pInputBuffer, pDisplayBuffer);

  INT vKey = NCursesToVKey(getch());
  
  if (vKey >= 32 && vKey <= 126) 
    {
    // standard printable ASCII key
    
    if (pInputBuffer->IsSelectionValid ())
      {
      cmdManager.ExecuteCommand ("SelectionDelete", NULL);
      }
    else
      {
      if (editorSettings.IsInsertMode ())
        {
        pInputBuffer->InsertChar(vKey);
        }
      else
        {
        pInputBuffer->ReplaceChar(vKey);
        }
      }
    }
  else
    {
    
    // Note: This needs to be converted to a configureable hotkey lookup.
    switch (vKey)
      {
      case VKEY_DOWN:      cmdManager.ExecuteCommand ("CursorDown", NULL);  break;
      case VKEY_UP:        cmdManager.ExecuteCommand ("CursorUp", NULL);  break;
      case VKEY_PAGEDOWN:  cmdManager.ExecuteCommand ("CursorPageDown", NULL);  break;
      case VKEY_PAGEUP:    cmdManager.ExecuteCommand ("CursorPageUp", NULL);  break;
      case VKEY_LEFT:      cmdManager.ExecuteCommand ("CursorLeft", NULL);  break;
      case VKEY_RIGHT:     cmdManager.ExecuteCommand ("CursorRight", NULL);  break;
      case VKEY_TAB:       pInputBuffer->InsertString (editorSettings.GetTabString()); 
      case VKEY_HOME:      cmdManager.ExecuteCommand ("CursorStartLine", NULL);  break;
      case VKEY_END:       cmdManager.ExecuteCommand ("CursorEndLine", NULL);  break;
      case VKEY_BACKSPACE: cmdManager.ExecuteCommand ("Backspace", NULL);  break;
      case VKEY_DELETE:    cmdManager.ExecuteCommand ("Delete", NULL);  break;
      case VKEY_INSERT:    editorSettings.SetInsertMode (!editorSettings.IsInsertMode());  break;
      case VKEY_ENTER:     
        {
        if (entryFieldHandler.IsActive ())
          {
          entryFieldHandler.HandleEnterKey (cmdManager);
          }
        else
          {
          pInputBuffer->InsertChar('\n'); 
          CursorIndent ();
          };
        };
        break;
      
      case (VKFLG_CONTROL | VKEY_RIGHT): cmdManager.ExecuteCommand ("CursorNextWord", NULL);  break;
      case (VKFLG_CONTROL | VKEY_LEFT):  cmdManager.ExecuteCommand ("CursorPrevWord", NULL);  break;
      
      case (VKFLG_CONTROL | VKEY_UP):    cmdManager.ExecuteCommand ("ScrollWindowUp", NULL);  break;
      case (VKFLG_CONTROL | VKEY_DOWN):  cmdManager.ExecuteCommand ("ScrollWindowDown", NULL);  break;
      
      case (VKFLG_CONTROL | VKFLG_SHIFT | VKEY_RIGHT): cmdManager.ExecuteCommand ("SelectNextWord", NULL);  break;
      case (VKFLG_CONTROL | VKFLG_SHIFT | VKEY_LEFT):  cmdManager.ExecuteCommand ("SelectPrevWord", NULL);  break;
      case (VKFLG_SHIFT | VKEY_LEFT):   cmdManager.ExecuteCommand ("SelectLeft", NULL);  break;
      case (VKFLG_SHIFT | VKEY_RIGHT):  cmdManager.ExecuteCommand ("SelectRight", NULL);  break;
      case (VKFLG_SHIFT | VKEY_UP):     cmdManager.ExecuteCommand ("SelectUp", NULL);  break;
      case (VKFLG_SHIFT | VKEY_DOWN):   cmdManager.ExecuteCommand ("SelectDown", NULL);  break;
      case (CTRL_A):                    cmdManager.ExecuteCommand ("SelectAll", NULL);  break;
      case (CTRL_G):                    cmdManager.ExecuteCommand ("GotoLinePrompt", NULL);  break;
      case (CTRL_F):                    cmdManager.ExecuteCommand ("FindTextPrompt", NULL);  break;
      case (VKFLG_ALT | VKEY_UP):       cmdManager.ExecuteCommand ("FindTextPrev", NULL);  break;
      case (VKFLG_ALT | VKEY_DOWN):     cmdManager.ExecuteCommand ("FindTextNext", NULL);  break;
      case (CTRL_P):                    ToggleFileListDisplay (); break;
      case (CTRL_S):                    cmdManager.ExecuteCommand ("Save", NULL);  break;
      case (CTRL_W):                    cmdManager.ExecuteCommand ("SaveAll", NULL);  break;
      
      case (CTRL_Q):
        return (TRUE);
      };
    };
  return (FALSE);
  };
  
//-----------------------------------------------------------------------------
INT  NCursesShell::NumDigits (INT  iValueIn)
  {
  INT  iCounter = 1;
  INT  iNumDigits = 0;
  while (iValueIn >= iCounter) 
    {
    ++iNumDigits;
    iCounter *= 10;
    }
  return (iNumDigits);
  }

//-----------------------------------------------------------------------------
VOID NCursesShell::DisplayWindow (INT                  iScreenX, 
                                  INT                  iScreenY,
                                  INT                  iWidth,
                                  INT                  iHeight, 
                                  GapBuffer *          pBuffer,
                                  EditorSettings &     editorSettings,
                                  EntryFieldHandler &  entryFieldHandler)
  {
  SyntaxParser  syntaxParser;

  Location  locCursor = pBuffer->GetCursor();
  Location  locBufferCurr;
  INT  iScreenCol = 0;
  INT  iMaxLine = pBuffer->GetNumLines () + 1;
  const INT LINE_BUFFER_LENGTH = 512;
  static char szLine [LINE_BUFFER_LENGTH];
  static INT  szFullMarkup [LINE_BUFFER_LENGTH];
  BOOL  bGetStatusBarInput = entryFieldHandler.IsActive ();
  BOOL  bDrawStatusBar = bShowStatusBar || bGetStatusBarInput;
  
  if (bDrawStatusBar)
    {
    iHeight -= 1;
    }
    
  // bounds check the window position
  Location  locWindowPosition = pBuffer->GetWindowPos ();
  INT  iTopLine = locWindowPosition.iLine;
  INT  iTopCol = locWindowPosition.iCol;
  
  iTopLine = TMax (1, TMin (iMaxLine - iHeight, iTopLine));
  iTopCol  = TMax (0, iTopCol);
  locWindowPosition.Set (iTopLine, iTopCol);
  pBuffer->SetWindowPos (locWindowPosition);
  
  // Draw the line numbers
  if (bShowLineNumbers) 
    {
    INT  iMaxLineDigits = NumDigits (iMaxLine);
    for (INT  iScreenLine = 0; iScreenLine < iHeight; ++iScreenLine)
      {
      locBufferCurr.iLine = iScreenLine + iTopLine;
      move(iScreenLine + iScreenY,iScreenX);
      
      if (locBufferCurr.iLine < iMaxLine)
        {
        INT  iCurrDigits  = NumDigits (locBufferCurr.iLine);
        INT  iCurrPadding = iMaxLineDigits - iCurrDigits;
        INT  iDivisor = 1;
        while (iCurrDigits > 1)
          {
          iDivisor *= 10;
          --iCurrDigits;
          };
        // padding
        for (; iCurrPadding > 0; --iCurrPadding)
          {
          addch (' ' | COLOR_PAIR(COLOR_WHITE) | A_REVERSE);
          }
        // line number
        INT  iDigit;
        for (; iDivisor > 0; iDivisor /= 10)
          {
          iDigit = locBufferCurr.iLine / iDivisor;
          locBufferCurr.iLine -= iDigit * iDivisor;
          addch (('0' + iDigit) | COLOR_PAIR(COLOR_WHITE) | A_REVERSE);
          }
        // marking column  (TODO)
        addch (' ' | COLOR_PAIR(COLOR_WHITE) | A_REVERSE);
        }
      else
        {
        for (INT iIndex = 0; iIndex <= iMaxLineDigits; ++iIndex)
          {
          addch (' ' | COLOR_PAIR(COLOR_WHITE) | A_REVERSE);
          };
        }
        
      }
    iScreenX += iMaxLineDigits + 1;
    iWidth -= iMaxLineDigits + 1;
    }

  pBuffer->SetLinesPerPage (iHeight);
  pBuffer->SetColPerPage (iWidth);
  
  // Draw the main window
  // move to the starting point in the buffer
  //printf ("Processing buffer with %d lines on a screen sized %d x %d\n\n", iMaxLine, iHeight, iWidth);
  // step through each line to display
  Location locHighlightStart = pBuffer->GetSelectionStart ();
  Location locHighlightEnd   = pBuffer->GetSelectionEnd ();
  
  INT   iHighlightFlag;

  for (INT  iScreenLine = iScreenY; iScreenLine < (iScreenY + iHeight); ++iScreenLine)
    {
    // TODO: determine if we are in a comment
    
    locBufferCurr.iLine = (iScreenLine - iScreenY) + iTopLine;
    
    // check and see if we are on a valid buffer line
    if (locBufferCurr.iLine < iMaxLine)
      {
      INT  iLineLength = pBuffer->GetLine (locBufferCurr.iLine, szLine, LINE_BUFFER_LENGTH);
      szLine [iLineLength] = '\0';
      syntaxParser.MarkupLine (szLine, iLineLength, szFullMarkup);
      
      // display the line  
      iScreenCol = 0;
      move (iScreenLine, iScreenX);
      locBufferCurr.iCol = iTopCol;
      for (; (iTopCol + iScreenCol < iLineLength) && (iScreenCol < iWidth); ++iScreenCol, ++locBufferCurr.iCol)
        {
        iHighlightFlag = GetHighlightState (locBufferCurr, locHighlightStart, locHighlightEnd) ? A_REVERSE : 0;
        addch (szFullMarkup[iTopCol + iScreenCol] | iHighlightFlag);
        }
      for (; iScreenCol < iWidth; ++iScreenCol, ++locBufferCurr.iCol)
        {
        iHighlightFlag = GetHighlightState (locBufferCurr, locHighlightStart, locHighlightEnd) ? A_REVERSE : 0;
        addch (' ' | iHighlightFlag);
        }
      }
    else
      {
      move (iScreenLine, iScreenX);
      for (iScreenCol = 0; iScreenCol < iWidth; ++iScreenCol)
        {
        addch (' ');
        }      
      }
    }

  // display the status bar
  // [1]Filename/Command      [x]                  INS/OVR [xx] Line: 0000 / 00000 [xx] Col: 00[1]
  static char szColNum [128];
  static char szLineNum [128];
  static char szFilename [512];
  if (bDrawStatusBar)
    {
    snprintf (szColNum, sizeof (szColNum), "Col : %d", locCursor.iCol);
    snprintf (szLineNum, sizeof (szLineNum), "Line: %d / %d", locCursor.iLine, iMaxLine);
    int  iColLen = strlen (szColNum);
    int  iLineLen = strlen (szLineNum);

    int  iFilenameMax = iWidth - 1 - 1 - 3 - 2 - iLineLen - 2 - iColLen - 1;
    int  iFilenameStart = 1;
    int  iFilenameLen   = 0;
    

    
    INT  iStatusBarY = iScreenY + iHeight;
    attron (A_REVERSE);
    move (iStatusBarY, iScreenX);
    addch (' ');
    if (!bGetStatusBarInput) 
      {
      snprintf (szFilename, iFilenameMax, "%s", pBuffer->GetFileName ());
      iFilenameLen = strlen (szFilename);
      addstr (szFilename);
      }
    for (INT  iFill = 0; iFill < iFilenameMax - iFilenameLen; ++iFill)
      {
      addch (' ');
      }
      
    if (editorSettings.IsInsertMode ())
      {
      addstr (" INS  ");
      }
    else
      {
      addstr (" OVR  ");
      };
    addstr (szLineNum);
    addch (' ');
    addstr (szColNum);
    addch (' ');
    attroff (A_REVERSE);
    
    // draw status line entry field if needed  
    if (bGetStatusBarInput)
      {
      move (iStatusBarY, iScreenX + iFilenameStart);
      attron (A_REVERSE);
      addstr (entryFieldHandler.GetInputField ().AsChar ());
      addstr(": ");
      attroff (A_REVERSE);
      
      INT  iEntryFieldInputLength = entryFieldHandler.GetInputField ().GetLength ();
      
      DrawBufferEntryField (iScreenX + iFilenameStart + iEntryFieldInputLength + 2, 
                            iStatusBarY, 
                            iFilenameMax - iEntryFieldInputLength - 2,
                            entryFieldHandler.GetBuffer (),
                            editorSettings);
      }    
    };  
  
  
  //move (0,0);
  //printw("selectStart %d %d  cursor %d %d", locHighlightStart.iLine, locHighlightStart.iCol, locCursor.iLine, locCursor.iCol);
  
  // move the cursor to the correct place
  if (!entryFieldHandler.IsActive ())
    {
    if (locHighlightStart.IsValid ())
      {
      curs_set(0);
      move (-1, -1);
      }
    else
      {
      // move the ncurses cursor to our gap buffer cursor location
      curs_set(2);
      move (iScreenY + locCursor.iLine - iTopLine, iScreenX + locCursor.iCol - iTopCol);
      }
    };
    
    

  };


//-----------------------------------------------------------------------------
VOID  NCursesShell::DrawBufferEntryField (INT               iScreenX, 
                                          INT               iScreenY,
                                          INT               iWidth,
                                          GapBuffer &       buffer,
                                          EditorSettings &  editorSettings)
  {
  Location  locCursor = buffer.GetCursor();
  Location  locBufferCurr;
  INT  iScreenCol = 0;
  const INT LINE_BUFFER_LENGTH = 512;
  static char szLine [LINE_BUFFER_LENGTH];

  buffer.SetLinesPerPage (1);
  buffer.SetColPerPage (iWidth);
  
  Location locHighlightStart = buffer.GetSelectionStart ();
  Location locHighlightEnd   = buffer.GetSelectionEnd ();

  Location  locWindowPosition = buffer.GetWindowPos ();
  INT  iTopCol = locWindowPosition.iCol;  
  
  INT   iHighlightFlag;

  locBufferCurr.iLine = 1;
  
  INT  iLineLength = buffer.GetLine (locBufferCurr.iLine, szLine, LINE_BUFFER_LENGTH);
  szLine [iLineLength] = '\0';
  
  // display the line  
  iScreenCol = 0;
  move (iScreenY, iScreenX);
  locBufferCurr.iCol = iTopCol;
  for (; (iTopCol + iScreenCol < iLineLength) && (iScreenCol < iWidth); ++iScreenCol, ++locBufferCurr.iCol)
    {
    iHighlightFlag = GetHighlightState (locBufferCurr, locHighlightStart, locHighlightEnd) ? A_REVERSE : 0;
    addch (szLine[iTopCol + iScreenCol] | iHighlightFlag);
    }
  for (; iScreenCol < iWidth; ++iScreenCol, ++locBufferCurr.iCol)
    {
    iHighlightFlag = GetHighlightState (locBufferCurr, locHighlightStart, locHighlightEnd) ? A_REVERSE : 0;
    addch (' ' | iHighlightFlag);
    }
    
  // move the cursor to the correct place
  if (locHighlightStart.IsValid ())
    {
    curs_set(0);
    move (-1, -1);
    }
  else
    {
    // move the ncurses cursor to our gap buffer cursor location
    curs_set(2);
    move (iScreenY + locCursor.iLine - 1, iScreenX + locCursor.iCol - iTopCol);
    }
  }
  
  
//-----------------------------------------------------------------------------
BOOL  NCursesShell::GetHighlightState (Location &  locCurr,
                                       Location &  locBegin,
                                       Location &  locEnd)
  {                                      
  if ((locBegin.IsValid ())
      &&
      (locEnd.IsValid ())
      &&
      (locCurr >= locBegin) 
      &&
      (locCurr <= locEnd) 
     )
    {
    return (TRUE);
    };
  return (FALSE);
  };
  
  
//-----------------------------------------------------------------------------
INT  NCursesShell::NCursesToVKey (INT  nCursesKey)
  {

  move(1,0);
  printw("ncurses key: %d %x %o                             ", nCursesKey, nCursesKey, nCursesKey);
  
  if (nCursesKey >= 32 && nCursesKey <= 126) 
    {
    return (nCursesKey);
    };
    
  switch (nCursesKey) 
    {
    // TODO: These keys need to be handled better, possibly with libtermkey.  The below approach 
    //  is probably very terminal-implementation specific.
    case 0x22f:         return (VKFLG_CONTROL | VKEY_RIGHT);
    case 0x220:         return (VKFLG_CONTROL | VKEY_LEFT);
    case 0x235:         return (VKFLG_CONTROL | VKEY_UP);
    case 0x20c:         return (VKFLG_CONTROL | VKEY_DOWN);
    case 0x22a:         return (VKFLG_CONTROL | VKEY_PAGEUP);
    case 0x225:         return (VKFLG_CONTROL | VKEY_PAGEDOWN);
    case 0x230:         return (VKFLG_CONTROL | VKFLG_SHIFT | VKEY_RIGHT);
    case 0x221:         return (VKFLG_CONTROL | VKFLG_SHIFT | VKEY_LEFT);
    case 0x00a:         return (VKEY_ENTER);
    
    case 0x09:          return (VKEY_TAB); // replaces CTRL_I
    case KEY_DOWN:      return (VKEY_DOWN);
    case KEY_UP:        return (VKEY_UP);
    case KEY_LEFT:      return (VKEY_LEFT);
    case KEY_RIGHT:     return (VKEY_RIGHT);
    case KEY_HOME:      return (VKEY_HOME);
    case KEY_END:       return (VKEY_END);
    case KEY_BACKSPACE: return (VKEY_BACKSPACE);
    case KEY_DC:        return (VKEY_DELETE);
    case KEY_IC:        return (VKEY_INSERT);
    case KEY_ENTER:     return (VKEY_NUMPAD_ENTER);   // enter on numpad
//    case KEY_TAB:       return (VKEY_TAB);
    case KEY_NPAGE:     return (VKEY_PAGEDOWN);
    case KEY_PPAGE:     return (VKEY_PAGEUP);
    case KEY_SLEFT:     return (VKFLG_SHIFT | VKEY_LEFT);
    case KEY_SRIGHT:    return (VKFLG_SHIFT | VKEY_RIGHT);
    case KEY_SR:        return (VKFLG_SHIFT | VKEY_UP);
    case KEY_SF:        return (VKFLG_SHIFT | VKEY_DOWN);
    
    case 0x233:         return (VKFLG_ALT | VKEY_UP);
    case 0x20a:         return (VKFLG_ALT | VKEY_DOWN);
    case 0x22d:         return (VKFLG_ALT | VKEY_RIGHT);
    case 0x21e:         return (VKFLG_ALT | VKEY_LEFT);
    
    default:
      {
      if (nCursesKey >= 1 && nCursesKey <= 26)
        {
        return (CTRL_A + nCursesKey - 1);
        }
      
      if (nCursesKey == KEY_F(1))        return (VKEY_F1);
      else if (nCursesKey == KEY_F(2))   return (VKEY_F2);
      else if (nCursesKey == KEY_F(3))   return (VKEY_F3);
      else if (nCursesKey == KEY_F(4))   return (VKEY_F4);
      else if (nCursesKey == KEY_F(5))   return (VKEY_F5);
      else if (nCursesKey == KEY_F(6))   return (VKEY_F6);
      else if (nCursesKey == KEY_F(7))   return (VKEY_F7);
      else if (nCursesKey == KEY_F(8))   return (VKEY_F8);
      else if (nCursesKey == KEY_F(9))   return (VKEY_F9);
      else if (nCursesKey == KEY_F(10))  return (VKEY_F10);
      else if (nCursesKey == KEY_F(11))  return (VKEY_F11);
      else if (nCursesKey == KEY_F(12))  return (VKEY_F12);
      else if (nCursesKey == KEY_F(13))  return (VKEY_F13);
      else if (nCursesKey == KEY_F(14))  return (VKEY_F14);
      else if (nCursesKey == KEY_F(15))  return (VKEY_F15);
      }
      break;
    };
  return (0);
  };
  
  