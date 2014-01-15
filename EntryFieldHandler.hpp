//
// EntryFieldHandler.hpp
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

#ifndef ENTRYFIELDHANDLER_HPP
#define ENTRYFIELDHANDLER_HPP

#include "Types.hpp"
#include "IntArray.hpp"
#include "RStr.hpp"
#include "RStrArray.hpp"
#include "Command.hpp"

//=============================================================================
class EntryFieldHandler 
  {
  private:
  
    RStrArray    astrInput;
    RStrArray    astrOutput;
    char         szTempString [256];
    GapBuffer    buffer;
    RStr         strFinishedCommand;    
    

  public:
  
    EntryFieldHandler  ();
    ~EntryFieldHandler ();
    
    BOOL          IsActive         (VOID);
    
    GapBuffer &   GetBuffer        (VOID)                           {return buffer;};
    
    RStr &        GetInputField    (VOID)                           {return (astrInput[astrOutput.Length ()]);};
  
    VOID          StartInput       (const char *  szCommand,
                                    RStrArray &   astrInputFields);
                                
    BOOL          HandleEnterKey   (CommandManager &  cmdManager);  
  };

#endif // ENTRYFIELDHANDLER_HPP