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


#ifndef COMMAND_HPP
#define COMMAND_HPP

#include "Types.hpp"
#include "RStr.hpp"
#include "RStrArray.hpp"

//------------------------------------------------------------------------
class Command 
  {
  private:
    RStr       strName;
    VOID (*fn)(RStrArray *); 
    
  public:
  
    Command *  pNext;
  
  public:
    Command ();
    ~Command ();
    
    VOID Execute (RStrArray *  arrayParams);
    
    VOID         SetName (const char *  szNameIn)     {strName = szNameIn; };
    const char * GetName (VOID)                       {return (strName.AsChar ());};
    VOID         SetFn   (VOID (*fnIn)(RStrArray *))  {fn = fnIn;};

  };
  
//------------------------------------------------------------------------
class CommandManager
  {
  private:
    Command *  pCommands;
  
  public:
               CommandManager  ();
               
               ~CommandManager ();
               
    VOID       ParseCommand    (RStr &       strIn,
                                RStr &       strNameOut,
                                RStrArray *  arrayParamsOut);
                             
    VOID       FreeCommands    (VOID);                    
    
    VOID       AddCommand      (const char *  szNameIn,
                                VOID (*fnIn)(RStrArray *));
                        
    Command *  FindCommand     (const char *  szNameIn);
    
    VOID ExecuteCommand        (const char *  szNameIn, 
                                RStrArray *   arrayParams);
    
  };
  
#endif // COMMAND_HPP