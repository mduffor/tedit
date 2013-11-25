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
  };          

//-----------------------------------------------------------------------------
NCursesShell::~NCursesShell ()
  {
  endwin ();  // end ncurses mode
  };

//-----------------------------------------------------------------------------
VOID NCursesShell::Update (GapBuffer *  pBuffer)
  {
  if (pBuffer == NULL)
    {
    // no buffer.  Error
    return;
    };
  SyntaxParser  syntaxParser;
  
  printw ("Testing ncurses.  Press a key to see it in bold. \n");
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
   
   int  iRowMax = 0;
   int  iColMax = 0;
   getmaxyx (stdscr, iRowMax, iColMax);
   
   move(0,0);
   // addch (ch | A_BOLD | A_UNDERLINE);
   
   int  iTopLine = 0;
   int  iTopCol = 0;
   int  iMaxLine = pBuffer->GetNumLines () - 1;
   const int LINE_BUFFER_LENGTH = 512;
   static char szLine [LINE_BUFFER_LENGTH];
   static int  szFullMarkup [LINE_BUFFER_LENGTH];
   
   // move to the starting point in the buffer
   
   // step through each line to display
   for (INT  iScreenLine = 0; iScreenLine < iRowMax; ++iScreenLine)
     {
     // determine if we are in a comment
     
     // check and see if we are on a valid buffer line
     if (iScreenLine + iTopLine <= iMaxLine)
       {
       INT  iLineLength = pBuffer->GetLine (iScreenLine + iTopLine, szLine, LINE_BUFFER_LENGTH);
       
       // parse the syntax of the line
       INT  iIndex = 0;
       while (iIndex < iLineLength)
         {
         // skip whitespace
         if ((szLine[iIndex] == ' ') || (szLine[iIndex] == '\t'))
           {
           while ((szLine[iIndex] == ' ') || (szLine[iIndex] == '\t'))
             {
             szFullMarkup[iIndex] = (int)szLine[iIndex];
             ++iIndex;
             }
           continue;
           }
         // parse character
         INT  iNumCharsParsed = 0;
         INT  iAttr = syntaxParser.Parse (&szLine[iIndex], iLineLength - iIndex, iNumCharsParsed);
         for (INT  iAttrIndex = 0; iAttrIndex < iNumCharsParsed; ++iAttrIndex)
           {
           szFullMarkup[iIndex] = szLine[iIndex] | iAttr;
           };
           
         };
         
       // display the line  
         
       }
     }
     
   refresh();
   getch();
  };
  
/*
// Syntax parsing


  word
  reserved word
  syntax
  comment
  number
  quote string
  escaped char in quote string
  preprocessor directive



*/