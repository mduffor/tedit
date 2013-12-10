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

#include <string.h>
#include "Types.hpp"
#include "Debug.hpp"
ASSERTFILE (__FILE__)

#include <ncurses.h>
#include "NCursesShell.hpp"
#include "GapBuffer.hpp"
#include "SyntaxParser.hpp"

//-----------------------------------------------------------------------------
NCursesShell::NCursesShell ()
  {
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
VOID NCursesShell::Update (GapBuffer *  pBuffer)
  {
  printf ("NCursesShell::Update ()\n");
  
  if (pBuffer == NULL)
    {
    // no buffer.  Error
    return;
    };
  int  iRowMax = 0;
  int  iColMax = 0;
  getmaxyx (stdscr, iRowMax, iColMax);
    
    
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
   

  
  //move(0,0);
  // addch (ch | A_BOLD | A_UNDERLINE);
  
  
  DisplayWindow (5, 5,
                 30, 20, // iColMax, iRowMax, 
                 pBuffer);
    
  refresh();
  getch();
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
VOID NCursesShell::DisplayWindow (INT          iScreenX, 
                                  INT          iScreenY,
                                  INT          iWidth,
                                  INT          iHeight, 
                                  GapBuffer *  pBuffer)
  {
  SyntaxParser  syntaxParser;

  Location  locCursor = pBuffer->GetCursor();
  int  iBufferLine = 0;
  int  iTopLine = 0;
  int  iTopCol = 0;
  INT  iScreenCol = 0;
  int  iMaxLine = pBuffer->GetNumLines () - 1;
  const int LINE_BUFFER_LENGTH = 512;
  static char szLine [LINE_BUFFER_LENGTH];
  static int  szFullMarkup [LINE_BUFFER_LENGTH];
  
  if (bShowLineNumbers) 
    {
    INT  iMaxLineDigits = NumDigits (iMaxLine);
    for (INT  iScreenLine = 0; iScreenLine < iHeight; ++iScreenLine)
      {
      iBufferLine = iScreenLine + iTopLine + 1;
      if (iBufferLine < iMaxLine)
        {
        move(iScreenLine + iScreenY,iScreenX);
        
        INT  iCurrDigits  = NumDigits (iBufferLine);
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
          iDigit = iBufferLine / iDivisor;
          iBufferLine -= iDigit * iDivisor;
          addch (('0' + iDigit) | COLOR_PAIR(COLOR_WHITE) | A_REVERSE);
          }
        // marking column  (TODO)
        addch (' ' | COLOR_PAIR(COLOR_WHITE) | A_REVERSE);
        }
        
      }
    iScreenX += iMaxLineDigits + 1;
    iWidth -= iMaxLineDigits + 1;
    }
  
  // move to the starting point in the buffer
  //printf ("Processing buffer with %d lines on a screen sized %d x %d\n\n", iMaxLine, iHeight, iWidth);
  // step through each line to display
  for (INT  iScreenLine = iScreenY; iScreenLine < (iScreenY + iHeight); ++iScreenLine)
    {
    // determine if we are in a comment
    
    iBufferLine = iScreenLine + iTopLine + 1;
    
    // check and see if we are on a valid buffer line
    if (iBufferLine < iMaxLine)
      {
      INT  iLineLength = pBuffer->GetLine (iBufferLine, szLine, LINE_BUFFER_LENGTH);
      szLine [iLineLength] = '\0';
      syntaxParser.MarkupLine (szLine, iLineLength, szFullMarkup);
      
      // display the line  
      iScreenCol = 0;
      move(iScreenLine,iScreenX);
      for (; (iTopCol + iScreenCol < iLineLength) && (iScreenCol < iWidth); ++iScreenCol)
        {
        addch (szFullMarkup[iTopCol + iScreenCol]);
        }
      for (; iScreenCol < iWidth; ++iScreenCol)
        {
        addch (' ');
        }
      }
    }

  // move the ncurses cursor to our gap buffer cursor location
  move(locCursor.iLine + iScreenY - iTopLine - 1, locCursor.iCol + iScreenX - iTopCol);
  };
  