//
// EntryFieldHandler.cpp
// Copyright 2014  Michael T. Duffy.  
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

#include "Types.hpp"
#include "Debug.hpp"
ASSERTFILE (__FILE__)

#include "GapBuffer.hpp"
#include "EntryFieldHandler.hpp"


//-----------------------------------------------------------------------------
EntryFieldHandler::EntryFieldHandler (VOID)
  {
  buffer.AllocBuffer (256);
  buffer.SetName ("EntryFieldBuffer");
  }
  
//-----------------------------------------------------------------------------
EntryFieldHandler::~EntryFieldHandler (VOID)
  {
  }

//-----------------------------------------------------------------------------
BOOL  EntryFieldHandler::IsActive (VOID)
  {
  return ((astrInput.Length () > 0) && (astrInput.Length () > astrOutput.Length()));
  }

//-----------------------------------------------------------------------------
VOID  EntryFieldHandler::StartInput  (const char *  szCommand,
                                      RStrArray &   astrInputFields)
  {
  astrInput.Clear ();
  astrOutput.Clear ();
  
  astrInput = astrInputFields;
  strFinishedCommand = szCommand;
  buffer.Clear();
  buffer.SetCursor (1,0);
  };

//-----------------------------------------------------------------------------
BOOL  EntryFieldHandler::HandleEnterKey (CommandManager &  cmdManager)
  {
  // return true if we handled the keypress
  
  INT  iInIndex  = astrInput.Length ();
  INT  iOutIndex = astrOutput.Length ();
  BOOL  bEntryFieldActive = (iInIndex > 0) && (iInIndex > iOutIndex);
  
  if (bEntryFieldActive) 
    {
    buffer.SetCursor (1,0);
    buffer.GetString (szTempString, sizeof (szTempString));
    astrOutput.SetLength (iOutIndex + 1);
    astrOutput[iOutIndex].Set (szTempString);
    
    if (iInIndex == iOutIndex + 1)
      {
      // input complete 
      cmdManager.ExecuteCommand (strFinishedCommand, &astrOutput);
      }
    return (TRUE);
    }
  return (FALSE);
  }
  
  
  
  
  
  
  
  
  
  