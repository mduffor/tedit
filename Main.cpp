//
// Main.cpp
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
#include "GapBuffer.hpp"
#include "GapBufferManager.hpp"
#include "NCursesShell.hpp"


#include "Debug.hpp"
ASSERTFILE (__FILE__)

//-----------------------------------------------------------------------------
int main (int argc, char *argv[])
  {
  NCursesShell *  pShell = new NCursesShell ();
  printf ("Hello World\n");
  
  GapBufferManager *  pManager = new GapBufferManager;
  pManager->CreateBuffer ("TestBuffer");
  GapBuffer *  gapBuffer = pManager->GetBuffer ("TestBuffer");
  gapBuffer->AllocBuffer (16);
  gapBuffer->SetFileName ("/testBuffer.txt");
  //gapBuffer->FillGap ();
  //const char * szRawBuffer = gapBuffer->GetBuffer();

  gapBuffer->InsertChar ('a');
  gapBuffer->InsertString ("The quick brown \nfox jumped over\nthe two lazy dogs.");
  
  
  
  pShell->Update (gapBuffer);
  
  delete (pShell);
  return (0);
  }
  
  