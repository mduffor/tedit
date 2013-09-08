//
// Command.cpp
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

//-----------------------------------------------------------------------------
Command::Command ()
  {
  strName.Reset ();
  fn = NULL;
  pNext = NULL;
  };
  
//-----------------------------------------------------------------------------
Command::~Command ()
  {
  };
  
//-----------------------------------------------------------------------------
VOID Command::Execute (RStrArray *  arrayParams)
  {
  if (fn != NULL)
    {
    fn (arrayParams);
    }
  };
  
//-----------------------------------------------------------------------------
CommandManager::CommandManager ()
  {
  pCommands = NULL;
  };
  
//-----------------------------------------------------------------------------
CommandManager::~CommandManager ()
  {
  FreeCommands ();
  };
  
//-----------------------------------------------------------------------------
VOID  CommandManager::ParseCommand (RStr &       strIn,
                                    RStr &       strNameOut,
                                    RStrArray *  arrayParamsOut)
  {
  };
  
//-----------------------------------------------------------------------------
VOID  CommandManager::FreeCommands (VOID)
  {
  Command *  pcmdToDelete = NULL;
  while (pCommands != NULL)
    {
    pcmdToDelete = pCommands;
    pCommands = pCommands->pNext;
    delete pcmdToDelete;
    }
  };

//-----------------------------------------------------------------------------
VOID  CommandManager::AddCommand (const char *  szNameIn,
                                  VOID (*fnIn)(RStrArray *))
  {
  Command *  pcmdNew = new Command;
  pcmdNew->SetName (szNameIn);
  pcmdNew->SetFn (fnIn);
  pcmdNew->pNext = pCommands;
  pCommands = pcmdNew;
  };

//-----------------------------------------------------------------------------
Command *  CommandManager::FindCommand (const char *  szNameIn)
  {
  Command *  pcmdSearch = pCommands;
  while (pcmdSearch != NULL)
    {
    if (strcmp (pcmdSearch->GetName (), szNameIn) == 0)
      {
      break;
      }
    pcmdSearch = pcmdSearch->pNext;
    };
  return (pcmdSearch);  
  };

//-----------------------------------------------------------------------------
VOID  CommandManager::ExecuteCommand (const char *  szNameIn, 
                                      RStrArray *   arrayParams)
  {
  Command *  pcmdToExecute = FindCommand (szNameIn);
  if (pcmdToExecute == NULL)
    {
    DBG_ERROR ("CommandManager::ExecuteCommand - Unable to find command : %s", szNameIn);
    return;
    }
  pcmdToExecute->Execute (arrayParams);
  };
  